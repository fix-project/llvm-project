//===-- Unittests for WASI stub semantics ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "hdr/errno_macros.h"
#include "src/pthread/pthread_create.h"
#include "src/signal/kill.h"
#include "src/signal/raise.h"
#include "src/signal/sigaction.h"
#include "src/sys/mman/mmap.h"
#include "src/sys/mman/munmap.h"
#include "src/unistd/fork.h"
#include "src/unistd/geteuid.h"
#include "src/unistd/getpid.h"
#include "src/unistd/getppid.h"
#include "src/unistd/getuid.h"
#include "test/UnitTest/Test.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

// WASI has no processes, no threads, and no memory mapping facilities.
// These tests pin the documented stub semantics of the WASI port.

TEST(LlvmLibcWasiStubTest, GetpidAndGetppidAreOne) {
  // A WASI instance is the only process; report pid 1 with no parent.
  EXPECT_EQ(LIBC_NAMESPACE::getpid(), static_cast<pid_t>(1));
  EXPECT_EQ(LIBC_NAMESPACE::getppid(), static_cast<pid_t>(1));
}

TEST(LlvmLibcWasiStubTest, UidsAreZero) {
  EXPECT_EQ(LIBC_NAMESPACE::getuid(), static_cast<uid_t>(0));
  EXPECT_EQ(LIBC_NAMESPACE::geteuid(), static_cast<uid_t>(0));
}

TEST(LlvmLibcWasiStubTest, ForkIsUnsupported) {
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::fork(), -1);
  EXPECT_EQ(errno, ENOSYS);
}

TEST(LlvmLibcWasiStubTest, KillSelfAndUnknownTargets) {
  // Signalling the (only) process with 0 succeeds; unknown pids report
  // ESRCH; invalid signal numbers report EINVAL.  Fatal delivery would
  // terminate the test process and is exercised manually.
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::kill(1, 0), 0);
  EXPECT_EQ(errno, 0);
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::kill(999, SIGTERM), -1);
  EXPECT_EQ(errno, ESRCH);
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::kill(1, 99), -1);
  EXPECT_EQ(errno, EINVAL);
}

static int g_raise_hits = 0;
static void raise_handler(int sig) { g_raise_hits += sig; }

TEST(LlvmLibcWasiStubTest, RaiseDeliversSynchronously) {
  struct sigaction act = {};
  act.sa_handler = raise_handler;
  ASSERT_EQ(LIBC_NAMESPACE::sigaction(SIGUSR1, &act, nullptr), 0);
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::raise(SIGUSR1), 0);
  EXPECT_EQ(errno, 0);
  EXPECT_EQ(g_raise_hits, SIGUSR1);

  struct sigaction ign = {};
  ign.sa_handler = SIG_IGN;
  ASSERT_EQ(LIBC_NAMESPACE::sigaction(SIGFPE, &ign, nullptr), 0);
  EXPECT_EQ(LIBC_NAMESPACE::raise(SIGFPE), 0);

  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::raise(99), -1);
  EXPECT_EQ(errno, EINVAL);
}

static void *noop_thread_main(void *arg) {
  (void)arg;
  return nullptr;
}

TEST(LlvmLibcWasiStubTest, PthreadCreateIsUnsupported) {
  pthread_t thread;
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::pthread_create(&thread, nullptr,
                                           noop_thread_main, nullptr),
            EAGAIN);
  EXPECT_EQ(errno, EAGAIN);
}

TEST(LlvmLibcWasiStubTest, AnonymousMmap) {
  errno = 0;
  void *result = LIBC_NAMESPACE::mmap(nullptr, 4096, PROT_READ | PROT_WRITE,
                                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_NE(result, MAP_FAILED);
  EXPECT_EQ(LIBC_NAMESPACE::munmap(result, 4096), 0);

  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::munmap(nullptr, 4096), -1);
  EXPECT_EQ(errno, EINVAL);
}
