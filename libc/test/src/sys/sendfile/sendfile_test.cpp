//===-- Unittests for sendfile --------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/CPP/string_view.h"
#include "src/fcntl/open.h"
#include "src/sys/sendfile/sendfile.h"
#include "src/unistd/close.h"
#include "src/unistd/lseek.h"
#include "src/unistd/read.h"
#include "src/unistd/unlink.h"
#include "src/unistd/write.h"
#include "test/UnitTest/ErrnoCheckingTest.h"
#include "test/UnitTest/ErrnoSetterMatcher.h"
#include "test/UnitTest/Test.h"

#include "hdr/fcntl_macros.h"
#include <sys/stat.h>
#include <unistd.h>

using namespace LIBC_NAMESPACE::testing::ErrnoSetterMatcher;
using LlvmLibcSendfileTest = LIBC_NAMESPACE::testing::ErrnoCheckingTest;
namespace cpp = LIBC_NAMESPACE::cpp;

TEST_F(LlvmLibcSendfileTest, CreateAndTransfer) {
  // The test strategy is to
  //   1. Create a temporary file with known data.
  //   2. Use sendfile to copy it to another file.
  //   3. Make sure that the data was actually copied.
  //   4. Clean up the temporary files.
  constexpr const char *IN_FILE = "testdata/sendfile_in.test";
  constexpr const char *OUT_FILE = "testdata/sendfile_out.test";
  const char IN_DATA[] = "sendfile test";
  constexpr ssize_t IN_SIZE = ssize_t(sizeof(IN_DATA));

  int in_fd = LIBC_NAMESPACE::open(IN_FILE, O_CREAT | O_WRONLY, S_IRWXU);
  ASSERT_GT(in_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  ASSERT_EQ(LIBC_NAMESPACE::write(in_fd, IN_DATA, IN_SIZE), IN_SIZE);
  ASSERT_THAT(LIBC_NAMESPACE::close(in_fd), Succeeds(0));

  in_fd = LIBC_NAMESPACE::open(IN_FILE, O_RDONLY);
  ASSERT_GT(in_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  int out_fd = LIBC_NAMESPACE::open(OUT_FILE, O_CREAT | O_WRONLY, S_IRWXU);
  ASSERT_GT(out_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  ssize_t size = LIBC_NAMESPACE::sendfile(out_fd, in_fd, nullptr, IN_SIZE);
  ASSERT_EQ(size, IN_SIZE);
  ASSERT_THAT(LIBC_NAMESPACE::close(in_fd), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::close(out_fd), Succeeds(0));

  out_fd = LIBC_NAMESPACE::open(OUT_FILE, O_RDONLY);
  ASSERT_GT(out_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  char buf[IN_SIZE];
  ASSERT_EQ(IN_SIZE, LIBC_NAMESPACE::read(out_fd, buf, IN_SIZE));
  ASSERT_EQ(cpp::string_view(buf), cpp::string_view(IN_DATA));

  ASSERT_THAT(LIBC_NAMESPACE::unlink(IN_FILE), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::unlink(OUT_FILE), Succeeds(0));
}

TEST_F(LlvmLibcSendfileTest, ExplicitOffsetDoesNotMoveFileOffset) {
  // With an explicit offset, sendfile must read from that offset, update it
  // by the number of bytes transferred, and leave the input file offset
  // untouched.
  constexpr const char *IN_FILE = "testdata/sendfile_in.test";
  constexpr const char *OUT_FILE = "testdata/sendfile_out.test";
  const char IN_DATA[] = "0123456789abcdef";
  constexpr ssize_t IN_SIZE = ssize_t(sizeof(IN_DATA));
  constexpr off_t START = 4;

  int in_fd = LIBC_NAMESPACE::open(IN_FILE, O_CREAT | O_WRONLY, S_IRWXU);
  ASSERT_GT(in_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  ASSERT_EQ(LIBC_NAMESPACE::write(in_fd, IN_DATA, IN_SIZE), IN_SIZE);
  ASSERT_THAT(LIBC_NAMESPACE::close(in_fd), Succeeds(0));

  in_fd = LIBC_NAMESPACE::open(IN_FILE, O_RDONLY);
  ASSERT_GT(in_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  int out_fd = LIBC_NAMESPACE::open(OUT_FILE, O_CREAT | O_WRONLY, S_IRWXU);
  ASSERT_GT(out_fd, 0);
  ASSERT_ERRNO_SUCCESS();

  off_t offset = START;
  ssize_t size = LIBC_NAMESPACE::sendfile(out_fd, in_fd, &offset, 8);
  ASSERT_EQ(size, static_cast<ssize_t>(8));
  ASSERT_EQ(offset, START + 8);
  // The input file offset must not have moved.
  ASSERT_EQ(LIBC_NAMESPACE::lseek(in_fd, 0, SEEK_CUR), static_cast<off_t>(0));

  ASSERT_THAT(LIBC_NAMESPACE::close(in_fd), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::close(out_fd), Succeeds(0));

  out_fd = LIBC_NAMESPACE::open(OUT_FILE, O_RDONLY);
  ASSERT_GT(out_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  char buf[9] = {};
  ASSERT_EQ(static_cast<ssize_t>(8), LIBC_NAMESPACE::read(out_fd, buf, 8));
  ASSERT_EQ(cpp::string_view(buf, 8), cpp::string_view(IN_DATA + START, 8));
  ASSERT_THAT(LIBC_NAMESPACE::close(out_fd), Succeeds(0));

  ASSERT_THAT(LIBC_NAMESPACE::unlink(IN_FILE), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::unlink(OUT_FILE), Succeeds(0));
}

TEST_F(LlvmLibcSendfileTest, NullOffsetAdvancesFileOffset) {
  // With a null offset, sendfile must read from the current input file
  // offset and advance it by exactly the number of bytes transferred.
  constexpr const char *IN_FILE = "testdata/sendfile_in.test";
  constexpr const char *OUT_FILE = "testdata/sendfile_out.test";
  const char IN_DATA[] = "0123456789abcdef";
  constexpr ssize_t IN_SIZE = ssize_t(sizeof(IN_DATA));
  constexpr off_t START = 2;
  constexpr size_t TRANSFER = 6;

  int in_fd = LIBC_NAMESPACE::open(IN_FILE, O_CREAT | O_WRONLY, S_IRWXU);
  ASSERT_GT(in_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  ASSERT_EQ(LIBC_NAMESPACE::write(in_fd, IN_DATA, IN_SIZE), IN_SIZE);
  ASSERT_THAT(LIBC_NAMESPACE::close(in_fd), Succeeds(0));

  in_fd = LIBC_NAMESPACE::open(IN_FILE, O_RDONLY);
  ASSERT_GT(in_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  ASSERT_EQ(LIBC_NAMESPACE::lseek(in_fd, START, SEEK_SET), START);
  int out_fd = LIBC_NAMESPACE::open(OUT_FILE, O_CREAT | O_WRONLY, S_IRWXU);
  ASSERT_GT(out_fd, 0);
  ASSERT_ERRNO_SUCCESS();

  ssize_t size =
      LIBC_NAMESPACE::sendfile(out_fd, in_fd, nullptr, TRANSFER);
  ASSERT_EQ(size, static_cast<ssize_t>(TRANSFER));
  ASSERT_EQ(LIBC_NAMESPACE::lseek(in_fd, 0, SEEK_CUR),
            START + static_cast<off_t>(TRANSFER));

  ASSERT_THAT(LIBC_NAMESPACE::close(in_fd), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::close(out_fd), Succeeds(0));

  out_fd = LIBC_NAMESPACE::open(OUT_FILE, O_RDONLY);
  ASSERT_GT(out_fd, 0);
  ASSERT_ERRNO_SUCCESS();
  char buf[TRANSFER + 1] = {};
  ASSERT_EQ(static_cast<ssize_t>(TRANSFER),
            LIBC_NAMESPACE::read(out_fd, buf, TRANSFER));
  ASSERT_EQ(cpp::string_view(buf, TRANSFER),
            cpp::string_view(IN_DATA + START, TRANSFER));
  ASSERT_THAT(LIBC_NAMESPACE::close(out_fd), Succeeds(0));

  ASSERT_THAT(LIBC_NAMESPACE::unlink(IN_FILE), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::unlink(OUT_FILE), Succeeds(0));
}
