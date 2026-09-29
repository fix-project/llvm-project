//===-- WASI implementation of sendfile -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// WASI has no native zero-copy sendfile operation, so this implementation
// copies the data through a user-space buffer with a pread/write loop.
//
//===----------------------------------------------------------------------===//

#include "src/sys/sendfile/sendfile.h"

#include "hdr/types/off_t.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "hdr/stdio_macros.h"
#include "src/unistd/lseek.h"
#include "src/unistd/pread.h"
#include "src/unistd/read.h"
#include "src/unistd/write.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, sendfile,
                   (int out_fd, int in_fd, off_t *offset, size_t count)) {
  constexpr size_t BUFFER_SIZE = 8192;
  unsigned char buffer[BUFFER_SIZE];

  off_t current_offset = 0;
  if (offset != nullptr)
    current_offset = *offset;

  size_t total = 0;
  while (total < count) {
    size_t remaining = count - total;
    size_t chunk = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;

    ssize_t n;
    if (offset != nullptr)
      n = pread(in_fd, buffer, chunk, current_offset);
    else
      n = read(in_fd, buffer, chunk);

    if (n < 0) {
      if (total > 0)
        break; // Data already transferred; report it and stop.
      return -1; // read() has already set libc_errno.
    }
    if (n == 0)
      break; // Reached end of file.

    ssize_t written = write(out_fd, buffer, static_cast<size_t>(n));
    bool write_failed = written < 0;
    if (write_failed)
      written = 0;

    // When the caller does not track the offset, read() has advanced the
    // input file offset past the bytes we read. If fewer bytes were
    // transferred than read, rewind so the untransferred bytes are not
    // silently skipped by a subsequent read.
    if (offset == nullptr && written < n)
      lseek(in_fd, static_cast<off_t>(written) - static_cast<off_t>(n),
            SEEK_CUR);

    total += static_cast<size_t>(written);
    if (offset != nullptr)
      current_offset += static_cast<off_t>(written);

    if (written < n) {
      if (total == 0) {
        if (!write_failed)
          libc_errno = EIO;
        return -1;
      }
      break; // Short write; the caller can resume from the reported offset.
    }
  }

  if (offset != nullptr && total > 0)
    *offset = current_offset;

  return static_cast<ssize_t>(total);
}

} // namespace LIBC_NAMESPACE_DECL
