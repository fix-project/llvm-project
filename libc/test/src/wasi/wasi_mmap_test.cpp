//===-- Tests for WASI mmap emulation -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/mmap.h"
#include "test/UnitTest/Test.h"

#include "hdr/errno_macros.h"
#include "hdr/fcntl_macros.h"
#include "src/fcntl/open.h"
#include "src/sys/mman/msync.h"
#include "src/sys/mman/munmap.h"
#include "src/sys/stat/fstat.h"
#include "src/unistd/close.h"
#include "src/unistd/unlink.h"
#include "src/unistd/write.h"

#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

TEST(LlvmLibcWasiMmapTest, SharedMappingPreservesFileSize) {
  constexpr char PATH[] = "/tmp/llvm_libc_wasi_mmap_test";
  LIBC_NAMESPACE::unlink(PATH);
  int fd = LIBC_NAMESPACE::open(PATH, O_CREAT | O_TRUNC | O_RDWR, 0600);
  ASSERT_GE(fd, 0);
  char data[8192] = {};
  ASSERT_EQ(LIBC_NAMESPACE::write(fd, data, sizeof(data)),
            static_cast<ssize_t>(sizeof(data)));

  void *mapping = LIBC_NAMESPACE::mmap(nullptr, 4096, PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fd, 0);
  ASSERT_NE(mapping, MAP_FAILED);
  static_cast<char *>(mapping)[0] = 'x';
  ASSERT_EQ(LIBC_NAMESPACE::msync(mapping, 4096, MS_SYNC), 0);
  struct stat st;
  ASSERT_EQ(LIBC_NAMESPACE::fstat(fd, &st), 0);
  EXPECT_EQ(st.st_size, static_cast<off_t>(8192));
  ASSERT_EQ(LIBC_NAMESPACE::munmap(mapping, 4096), 0);
  ASSERT_EQ(LIBC_NAMESPACE::close(fd), 0);
  ASSERT_EQ(LIBC_NAMESPACE::unlink(PATH), 0);
}

TEST(LlvmLibcWasiMmapTest, InvalidFileDescriptorFails) {
  void *mapping =
      LIBC_NAMESPACE::mmap(nullptr, 4096, PROT_READ, MAP_PRIVATE, -1, 0);
  EXPECT_EQ(mapping, MAP_FAILED);
  EXPECT_EQ(errno, EBADF);
}
