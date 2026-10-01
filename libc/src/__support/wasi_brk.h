//===-- Shared WASI linear-memory break state -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_WASI_BRK_H
#define LLVM_LIBC_SRC___SUPPORT_WASI_BRK_H

#include <stddef.h>
#include <stdint.h>

#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

// malloc and brk share one linear-memory high-water mark. An allocator growth
// advances the lowest break to which brk may subsequently retreat.
uintptr_t wasi_current_break();
bool wasi_set_break(uintptr_t address);
long wasi_allocator_grow(size_t pages);

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_WASI_BRK_H
