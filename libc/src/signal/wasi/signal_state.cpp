//===-- Shared state for WASI signals -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "signal_utils.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

SignalState &signal_state() {
  static SignalState state;
  return state;
}

bool is_valid_signum(int signum) {
  if (signum < 1 || signum > NSIG - 1)
    return false;
  // SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.
  if (signum == SIGKILL || signum == SIGSTOP)
    return false;
  return true;
}

bool default_terminates(int signum) {
  // POSIX default dispositions: these signals are ignored, everything else
  // terminates.  (SIGSTOP cannot be stopped on WASI; treat it as fatal.)
  switch (signum) {
  case SIGCHLD:
  case SIGCONT:
  case SIGURG:
  case SIGWINCH:
    return false;
  default:
    return true;
  }
}

void deliver_self(int signum) {
  SignalState &state = signal_state();
  if (signum != SIGKILL && signum != SIGSTOP) {
    if (state.actions[signum].sa_flags & SA_SIGINFO) {
      auto sa = state.actions[signum].sa_sigaction;
      if (sa != nullptr && reinterpret_cast<intptr_t>(sa) != 1) {
        siginfo_t info = {};
        state.actions[signum].sa_sigaction(signum, &info, nullptr);
        return;
      }
    } else {
      void (*handler)(int) = state.actions[signum].sa_handler;
      if (handler == SIG_IGN)
        return;
      if (handler != SIG_DFL && handler != nullptr) {
        handler(signum);
        return;
      }
    }
  }
  if (default_terminates(signum)) {
    // The WASI preview1 signal enum matches the POSIX signal numbers.
    __wasi_proc_raise(static_cast<__wasi_signal_t>(signum));
    // If the runtime does not implement proc_raise, exit with the signal
    // number (runtimes restrict exit statuses to 0..126).
    __wasi_proc_exit(static_cast<__wasi_errno_t>(signum));
  }
}

sigset_t empty_set() {
  sigset_t set = {};
  return set;
}

sigset_t full_set() {
  sigset_t set = {};
  for (int sig = 1; sig <= NSIG - 1; ++sig)
    set.__signals[(sig - 1) / (sizeof(unsigned long) * 8)] |=
        (1UL << ((sig - 1) % (sizeof(unsigned long) * 8)));
  return set;
}

bool add_signal(sigset_t &set, int signum) {
  // Match glibc semantics: bounds are the bit width of sigset_t, not NSIG.
  constexpr int max_signum = 8 * static_cast<int>(sizeof(sigset_t));
  if (signum < 1 || signum > max_signum)
    return false;
  set.__signals[(signum - 1) / (sizeof(unsigned long) * 8)] |=
      (1UL << ((signum - 1) % (sizeof(unsigned long) * 8)));
  return true;
}

bool delete_signal(sigset_t &set, int signum) {
  constexpr int max_signum = 8 * static_cast<int>(sizeof(sigset_t));
  if (signum < 1 || signum > max_signum)
    return false;
  set.__signals[(signum - 1) / (sizeof(unsigned long) * 8)] &=
      ~(1UL << ((signum - 1) % (sizeof(unsigned long) * 8)));
  return true;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL
