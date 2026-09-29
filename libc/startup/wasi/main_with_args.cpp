//===-- WASI entry point for a main with arguments -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

extern "C" {
int __main_argc_argv(int, char **);
extern int start_argc;
extern char **start_argv;

// This archive member is linked only when main has arguments. A zero-argument
// main supplies __main_void itself, so its signature cannot conflict with
// either of the argument-taking entry points.
int __main_void() { return __main_argc_argv(start_argc, start_argv); }
}
