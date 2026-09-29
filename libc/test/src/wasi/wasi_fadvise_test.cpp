//===-- Tests for WASI posix_fadvise/posix_fallocate ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/fcntl/posix_fadvise.h"
#include "src/fcntl/posix_fallocate.h"
#include "test/UnitTest/Test.h"

#include "hdr/errno_macros.h"
#include "hdr/fcntl_macros.h"
#include "src/__support/macros/properties/types.h"
#include "src/fcntl/openat.h"
#include "src/unistd/close.h"
#include "src/unistd/unlink.h"

#define TEST_FILE "/tmp/posix_fadvise_test.txt"

class LlvmLibcWasiFadviseTest : public LIBC_NAMESPACE::testing::Test {
protected:
  void SetUp() override { LIBC_NAMESPACE::unlink(TEST_FILE); }
  void TearDown() override { LIBC_NAMESPACE::unlink(TEST_FILE); }
};

TEST_F(LlvmLibcWasiFadviseTest, InvalidArguments) {
  int fd = LIBC_NAMESPACE::openat(AT_FDCWD, TEST_FILE,
                                  O_CREAT | O_RDWR, 0644);
  ASSERT_GT(fd, 0);
  EXPECT_EQ(LIBC_NAMESPACE::posix_fadvise(fd, -1, 0, POSIX_FADV_NORMAL),
            EINVAL);
  EXPECT_EQ(LIBC_NAMESPACE::posix_fadvise(fd, 0, -1, POSIX_FADV_NORMAL),
            EINVAL);
  EXPECT_EQ(LIBC_NAMESPACE::posix_fallocate(fd, -1, 0), EINVAL);
  EXPECT_EQ(LIBC_NAMESPACE::posix_fallocate(fd, 0, -1), EINVAL);
  ASSERT_EQ(LIBC_NAMESPACE::close(fd), 0);
}

TEST_F(LlvmLibcWasiFadviseTest, ValidArguments) {
  int fd = LIBC_NAMESPACE::openat(AT_FDCWD, TEST_FILE,
                                  O_CREAT | O_RDWR, 0644);
  ASSERT_GT(fd, 0);
  // The return value is an error number (0 on success); runtimes may not
  // support every advice, but the call must never return -1.
  EXPECT_GE(LIBC_NAMESPACE::posix_fadvise(fd, 0, 0, POSIX_FADV_NORMAL), 0);
  EXPECT_GE(LIBC_NAMESPACE::posix_fallocate(fd, 0, 4096), 0);
  ASSERT_EQ(LIBC_NAMESPACE::close(fd), 0);
}
