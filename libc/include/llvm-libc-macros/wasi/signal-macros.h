//===-- Definition of WASI signal number macros ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_WASI_SIGNAL_MACROS_H
#define LLVM_LIBC_MACROS_WASI_SIGNAL_MACROS_H

#include "../../__llvm-libc-common.h"

// The standard POSIX signal numbers (1-31). WASI has no kernel signal
// delivery; these numbers exist so that programs remain link- and
// compile-compatible.
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPOLL SIGIO
#define SIGPWR 30
#define SIGSYS 31

// Max signal number. WASI does not support real-time signals.
#define NSIG 32

// The sigset_t is stored as an array of unsigned long words, one bit per
// signal (bit 0 is signal 1, and so on).
#define __NSIGSET_WORDS (NSIG / (sizeof(unsigned long) * 8))

#define SIG_BLOCK 0   // For blocking signals
#define SIG_UNBLOCK 1 // For unblocking signals
#define SIG_SETMASK 2 // For setting signal mask

// Flag values to be used for setting sigaction.sa_flags.
#define SA_NOCLDSTOP 0x00000001
#define SA_NOCLDWAIT 0x00000002
#define SA_SIGINFO 0x00000004
#define SA_RESTART 0x10000000
#define SA_ONSTACK 0x08000000
#define SA_NODEFER 0x40000000
#define SA_RESETHAND 0x80000000

// Signal stack flags
#define SS_ONSTACK 0x1
#define SS_DISABLE 0x2

#define MINSIGSTKSZ 2048
#define SIGSTKSZ 8192

#define SIG_ERR __LLVM_LIBC_CAST(reinterpret_cast, void (*)(int), -1)
#define SIG_DFL __LLVM_LIBC_CAST(reinterpret_cast, void (*)(int), 0)
// Wasm function table indexes 1 and 2 can name real handlers.
#define SIG_IGN __LLVM_LIBC_CAST(reinterpret_cast, void (*)(int), -2)
#define SIG_HOLD __LLVM_LIBC_CAST(reinterpret_cast, void (*)(int), -3)

#endif // LLVM_LIBC_MACROS_WASI_SIGNAL_MACROS_H
