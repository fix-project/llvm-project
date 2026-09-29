//===-- Internal header for WASI signals ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SIGNAL_WASI_SIGNAL_UTILS_H
#define LLVM_LIBC_SRC_SIGNAL_WASI_SIGNAL_UTILS_H

#include "hdr/signal_macros.h"
#include "hdr/types/sigset_t.h"
#include "hdr/types/stack_t.h"
#include "hdr/types/struct_sigaction.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// WASI has no signal delivery. Signal state is tracked purely so that
// sigaction/sigprocmask style bookkeeping behaves consistently for
// link-compatible programs. Handlers are stored but never fire.
// NOTE: single-threaded only (LIBC_THREAD_MODE_IS_SINGLE).
struct SignalState {
  struct sigaction actions[NSIG + 1] = {};
  sigset_t mask = {};
  stack_t altstack = {};
  bool altstack_set = false;
};

SignalState &signal_state();

// Returns true when signum is a valid, catchable signal number.
bool is_valid_signum(int signum);

// Returns true when the default disposition of `signum` is to terminate.
bool default_terminates(int signum);

// Synchronously delivers `signum` to the calling process: runs the installed
// handler, does nothing for SIG_IGN / ignore-by-default signals, and
// terminates the process for terminate-by-default signals.
void deliver_self(int signum);

sigset_t empty_set();
sigset_t full_set();
bool add_signal(sigset_t &set, int signum);
bool delete_signal(sigset_t &set, int signum);

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SIGNAL_WASI_SIGNAL_UTILS_H
