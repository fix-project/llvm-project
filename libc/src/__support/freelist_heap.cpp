//===-- Implementation for freelist_heap ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/freelist_heap.h"
#include "src/__support/macros/config.h"
#if defined(__wasi__)
#include "src/__support/wasi_brk.h"
#endif

#include <stddef.h>

namespace LIBC_NAMESPACE_DECL {

static LIBC_CONSTINIT FreeListHeap freelist_heap_symbols;
FreeListHeap *freelist_heap = &freelist_heap_symbols;

#if defined(__wasi__)
namespace {
constexpr uintptr_t WASM_PAGE_SIZE = 65536;
uintptr_t program_break = 0;
uintptr_t minimum_break = 0;
bool break_initialized = false;

void init_break() {
  if (break_initialized)
    return;
  program_break = static_cast<uintptr_t>(__builtin_wasm_memory_size(0)) *
                  WASM_PAGE_SIZE;
  minimum_break = program_break;
  break_initialized = true;
}
} // namespace

uintptr_t wasi_current_break() {
  init_break();
  return program_break;
}

bool wasi_set_break(uintptr_t address) {
  init_break();
  if (address < minimum_break)
    return false;
  size_t current_pages = __builtin_wasm_memory_size(0);
  size_t required_pages = address / WASM_PAGE_SIZE +
                          (address % WASM_PAGE_SIZE != 0);
  if (required_pages > current_pages &&
      __builtin_wasm_memory_grow(0, required_pages - current_pages) < 0)
    return false;
  program_break = address;
  return true;
}

long wasi_allocator_grow(size_t pages) {
  init_break();
  size_t current_pages = __builtin_wasm_memory_size(0);
  if (current_pages > UINTPTR_MAX / WASM_PAGE_SIZE ||
      pages > UINTPTR_MAX / WASM_PAGE_SIZE - current_pages)
    return -1;
  long previous_pages = __builtin_wasm_memory_grow(0, pages);
  if (previous_pages >= 0) {
    minimum_break = (static_cast<uintptr_t>(previous_pages) + pages) *
                    WASM_PAGE_SIZE;
    program_break = minimum_break;
  }
  return previous_pages;
}
#endif

} // namespace LIBC_NAMESPACE_DECL
