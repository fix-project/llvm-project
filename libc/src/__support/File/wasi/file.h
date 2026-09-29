//===--- WASI specialization of the File data structure -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_FILE_WASI_FILE_H
#define LLVM_LIBC_SRC___SUPPORT_FILE_WASI_FILE_H

#include "hdr/types/off_t.h"
#include "src/__support/File/file.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

FileIOResult wasi_file_write(File *, const void *, size_t);
FileIOResult wasi_file_read(File *, void *, size_t);
ErrorOr<off_t> wasi_file_seek(File *, off_t, int);
int wasi_file_close(File *);

class WasiFile : public File {
  int fd;
  bool heap_allocated;

public:
  constexpr WasiFile(int file_descriptor, uint8_t *buffer, size_t buffer_size,
                     int buffer_mode, bool owned, File::ModeFlags modeflags,
                     bool heap_allocated = true)
      : File(&wasi_file_write, &wasi_file_read, &wasi_file_seek,
             &wasi_file_close, buffer, buffer_size, buffer_mode, owned,
             modeflags),
        fd(file_descriptor), heap_allocated(heap_allocated) {}

  int get_fd() const { return fd; }
  bool is_heap_allocated() const { return heap_allocated; }
};

// Create a File object and associate it with a fd.
ErrorOr<WasiFile *> create_file_from_fd(int fd, const char *mode);

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_FILE_WASI_FILE_H
