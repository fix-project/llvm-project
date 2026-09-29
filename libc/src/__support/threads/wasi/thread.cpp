//===-- Implementation of a WASI thread class -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/threads/thread.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/config.h"

#include "hdr/errno_macros.h"

namespace LIBC_NAMESPACE_DECL {

int Thread::run(ThreadStyle style, ThreadRunner runner, void *arg, void *stack,
                size_t stacksize, size_t guardsize, bool detached) {
  // WASI preview1 is single-threaded; new threads cannot be created.
  (void)style;
  (void)runner;
  (void)arg;
  (void)stack;
  (void)stacksize;
  (void)guardsize;
  (void)detached;
  return EAGAIN;
}

int Thread::join(ThreadReturnValue &retval) {
  // No thread can have been created, so joining always fails.
  (void)retval;
  return EDEADLK;
}

int Thread::detach() {
  // No thread can have been created, so detaching always fails.
  return EDEADLK;
}

void Thread::wait() {}

bool Thread::operator==(const Thread &other) const {
  return attrib == other.attrib;
}

int Thread::set_name(const cpp::string_view &name) {
  (void)name;
  return 0;
}

int Thread::get_name(cpp::StringStream &name) const {
  (void)name;
  return ENOTSUP;
}

[[noreturn]] void thread_exit(ThreadReturnValue retval, ThreadStyle style) {
  (void)style;
  // In a single-threaded world, exiting the only thread exits the process.
  wasi::__wasi_proc_exit(
      static_cast<wasi::__wasi_errno_t>(retval.stdc_retval));
  __builtin_unreachable();
}

} // namespace LIBC_NAMESPACE_DECL
