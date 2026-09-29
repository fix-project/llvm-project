//===-- Tests for WASI path resolution ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdlib/realpath.h"
#include "test/UnitTest/Test.h"

#include "hdr/errno_macros.h"
#include "hdr/fcntl_macros.h"
#include "src/dirent/closedir.h"
#include "src/dirent/opendir.h"
#include "src/dirent/readdir.h"
#include "src/fcntl/open.h"
#include "src/sys/stat/mkdir.h"
#include "src/sys/stat/stat.h"
#include "src/unistd/close.h"
#include "src/unistd/rmdir.h"
#include "src/unistd/symlink.h"
#include "src/unistd/unlink.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace {

constexpr char DIR[] = "/tmp/llvm_libc_wasi_path_test";
constexpr char SUBDIR[] = "/tmp/llvm_libc_wasi_path_test/sub";
constexpr char FILE_PATH[] = "/tmp/llvm_libc_wasi_path_test/file";
constexpr char LINK[] = "/tmp/llvm_libc_wasi_path_test_link";

class LlvmLibcWasiPathTest : public LIBC_NAMESPACE::testing::Test {
protected:
  void SetUp() override {
    LIBC_NAMESPACE::unlink(LINK);
    LIBC_NAMESPACE::unlink(FILE_PATH);
    LIBC_NAMESPACE::rmdir(SUBDIR);
    LIBC_NAMESPACE::rmdir(DIR);
    ASSERT_EQ(LIBC_NAMESPACE::mkdir(DIR, 0700), 0);
    ASSERT_EQ(LIBC_NAMESPACE::mkdir(SUBDIR, 0700), 0);
    int fd = LIBC_NAMESPACE::open(FILE_PATH, O_CREAT | O_WRONLY, 0600);
    ASSERT_GE(fd, 0);
    ASSERT_EQ(LIBC_NAMESPACE::close(fd), 0);
    ASSERT_EQ(LIBC_NAMESPACE::symlink("llvm_libc_wasi_path_test/sub", LINK), 0);
  }

  void TearDown() override {
    LIBC_NAMESPACE::unlink(LINK);
    LIBC_NAMESPACE::unlink(FILE_PATH);
    LIBC_NAMESPACE::rmdir(SUBDIR);
    LIBC_NAMESPACE::rmdir(DIR);
  }
};

TEST_F(LlvmLibcWasiPathTest, SymlinkBeforeParentComponent) {
  char resolved[4096];
  ASSERT_STREQ(LIBC_NAMESPACE::realpath(
                   "/tmp/llvm_libc_wasi_path_test_link/../file", resolved),
               FILE_PATH);
  int fd = LIBC_NAMESPACE::open("/tmp/llvm_libc_wasi_path_test_link/../file",
                                O_RDONLY);
  ASSERT_GE(fd, 0);
  ASSERT_EQ(LIBC_NAMESPACE::close(fd), 0);
}

TEST_F(LlvmLibcWasiPathTest, MissingPathFails) {
  char resolved[4096];
  ASSERT_EQ(LIBC_NAMESPACE::realpath("/tmp/llvm_libc_wasi_path_test/missing",
                                     resolved),
            nullptr);
  ASSERT_EQ(errno, ENOENT);
}

TEST(LlvmLibcWasiMountTest, RootListsPreopenedDirectory) {
  struct stat st;
  ASSERT_EQ(LIBC_NAMESPACE::stat("/", &st), 0);
  ASSERT_TRUE(S_ISDIR(st.st_mode));
  ::DIR *dir = LIBC_NAMESPACE::opendir("/");
  ASSERT_NE(dir, nullptr);
  bool found_tmp = false;
  while (struct dirent *entry = LIBC_NAMESPACE::readdir(dir))
    if (strcmp(entry->d_name, "tmp") == 0)
      found_tmp = true;
  EXPECT_TRUE(found_tmp);
  EXPECT_EQ(LIBC_NAMESPACE::closedir(dir), 0);
}

} // namespace
