//===-- Unittests for epoll emulation on WASI -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "hdr/errno_macros.h"
#include "src/fcntl/open.h"
#include "src/sys/epoll/epoll_create.h"
#include "src/sys/epoll/epoll_create1.h"
#include "src/sys/epoll/epoll_ctl.h"
#include "src/sys/epoll/epoll_wait.h"
#include "src/sys/epoll/epoll_pwait2.h"
#include "src/unistd/close.h"
#include "src/unistd/unlink.h"
#include "test/UnitTest/ErrnoCheckingTest.h"
#include "test/UnitTest/ErrnoSetterMatcher.h"
#include "test/UnitTest/Test.h"

#include <sys/epoll.h>
#include <sys/stat.h>
#include <time.h>

using namespace LIBC_NAMESPACE::testing::ErrnoSetterMatcher;
using LlvmLibcEpollTest = LIBC_NAMESPACE::testing::ErrnoCheckingTest;

TEST_F(LlvmLibcEpollTest, CreateAndClose) {
  int epfd = LIBC_NAMESPACE::epoll_create1(0);
  ASSERT_GT(epfd, 0);
  ASSERT_ERRNO_SUCCESS();
  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));

  epfd = LIBC_NAMESPACE::epoll_create(1);
  ASSERT_GT(epfd, 0);
  ASSERT_ERRNO_SUCCESS();
  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));
}

TEST_F(LlvmLibcEpollTest, CreateInvalidArguments) {
  ASSERT_THAT(LIBC_NAMESPACE::epoll_create(0), Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_create(-1), Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_create1(12345), Fails(EINVAL));
}

TEST_F(LlvmLibcEpollTest, CtlInvalidArguments) {
  struct epoll_event ev = {};
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(-1, EPOLL_CTL_ADD, 0, &ev),
              Fails(EBADF));

  int epfd = LIBC_NAMESPACE::epoll_create1(0);
  ASSERT_GT(epfd, 0);

  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, 99, 0, &ev), Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_ADD, -1, &ev),
               Fails(EBADF));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_ADD, 12345, &ev),
              Fails(EBADF));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_ADD, epfd, &ev),
              Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_ADD, 0, nullptr),
              Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_DEL, 12345, nullptr),
              Fails(ENOENT));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_MOD, 12345, &ev),
              Fails(ENOENT));
  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));
}

TEST_F(LlvmLibcEpollTest, WaitInvalidArguments) {
  struct epoll_event ev;
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(-1, &ev, 1, 0), Fails(EBADF));

  int epfd = LIBC_NAMESPACE::epoll_create1(0);
  ASSERT_GT(epfd, 0);
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, nullptr, 1, 0), Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, &ev, 0, 0), Fails(EINVAL));
  timespec invalid = {0, 1000000000};
  ASSERT_THAT(LIBC_NAMESPACE::epoll_pwait2(epfd, &ev, 1, &invalid, nullptr),
              Fails(EINVAL));
  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));
}

TEST_F(LlvmLibcEpollTest, WaitEmptySetTimesOut) {
  int epfd = LIBC_NAMESPACE::epoll_create1(0);
  ASSERT_GT(epfd, 0);

  struct epoll_event ev;
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, &ev, 1, 10), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));
}

TEST_F(LlvmLibcEpollTest, WaitReportsReadyFile) {
  constexpr const char *FILENAME = "epoll.test";
  auto TEST_FILE = libc_make_test_file_path(FILENAME);
  int fd = LIBC_NAMESPACE::open(TEST_FILE, O_WRONLY | O_CREAT, S_IRWXU);
  ASSERT_GT(fd, 0);

  int epfd = LIBC_NAMESPACE::epoll_create1(0);
  ASSERT_GT(epfd, 0);

  struct epoll_event ev = {};
  ev.events = EPOLLOUT | EPOLLIN;
  ev.data.u64 = 0xCAFE;
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev),
              Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev),
              Fails(EEXIST));

  // Regular files are always ready for read/write.
  struct epoll_event out[4];
  int n = LIBC_NAMESPACE::epoll_wait(epfd, out, 4, 100);
  ASSERT_EQ(n, 1);
  EXPECT_EQ(out[0].data.u64, static_cast<uint64_t>(0xCAFE));
  EXPECT_TRUE((out[0].events & EPOLLOUT) != 0);

  ev.events = EPOLLOUT | EPOLLONESHOT;
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev),
              Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, out, 4, 100), Succeeds(1));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, out, 4, 0), Succeeds(0));
  // MOD re-arms a one-shot watch; it does not need to be added again.
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev),
              Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, out, 4, 100), Succeeds(1));

  // Remove the fd; the set is now empty.
  ASSERT_THAT(LIBC_NAMESPACE::epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr),
              Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, out, 4, 10), Succeeds(0));

  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::close(fd), Succeeds(0));
  ASSERT_THAT(LIBC_NAMESPACE::unlink(TEST_FILE), Succeeds(0));
}

TEST_F(LlvmLibcEpollTest, CloseReleasesInstance) {
  int epfd = LIBC_NAMESPACE::epoll_create1(0);
  ASSERT_GT(epfd, 0);
  ASSERT_THAT(LIBC_NAMESPACE::close(epfd), Succeeds(0));

  // After close, the handle no longer refers to an epoll instance.
  struct epoll_event ev;
  ASSERT_THAT(LIBC_NAMESPACE::epoll_wait(epfd, &ev, 1, 0), Fails(EBADF));
}
