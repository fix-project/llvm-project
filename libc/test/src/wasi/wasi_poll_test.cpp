//===-- Tests for WASI poll_oneoff-based poll ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/poll/poll.h"
#include "test/UnitTest/Test.h"

#include <poll.h>

TEST(LlvmLibcWasiPollTest, HeapBackedEvents) {
  pollfd fds[40];
  for (auto &fd : fds) {
    fd.fd = -1;
    fd.events = POLLOUT;
    fd.revents = -1;
  }
  fds[39].fd = 1;
  EXPECT_EQ(LIBC_NAMESPACE::poll(fds, 40, 0), 1);
  for (int i = 0; i < 39; ++i)
    EXPECT_EQ(fds[i].revents, static_cast<short>(0));
  EXPECT_NE(fds[39].revents & POLLOUT, 0);
}

TEST(LlvmLibcWasiPollTest, InvalidDescriptor) {
  pollfd fds[2] = {{123456, POLLIN, 0}, {1, POLLOUT, 0}};
  EXPECT_EQ(LIBC_NAMESPACE::poll(fds, 2, 100), 2);
  EXPECT_EQ(fds[0].revents, static_cast<short>(POLLNVAL));
  EXPECT_NE(fds[1].revents & POLLOUT, 0);
}
