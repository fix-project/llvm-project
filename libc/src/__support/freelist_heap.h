//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Interface for freelist_heap.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_FREELIST_HEAP_H
#define LLVM_LIBC_SRC___SUPPORT_FREELIST_HEAP_H

#include <stddef.h>

#include "block.h"
#include "freestore.h"
#include "src/__support/CPP/optional.h"
#include "src/__support/CPP/span.h"
#include "src/__support/libc_assert.h"
#include "src/__support/macros/config.h"
#include "src/__support/math_extras.h"
#include "src/string/memory_utils/inline_memcpy.h"
#include "src/string/memory_utils/inline_memset.h"

namespace LIBC_NAMESPACE_DECL {

#if defined(__wasi__)
// On WASI, wasm-ld provides __heap_base (end of the data/stack image) and
// __heap_end (end of the linear memory) as linker-defined symbols, but only
// for non-PIC links. They are declared weak so that a single libc.a can also
// be linked into shared objects, where the heap is bootstrapped at runtime
// (see init()).
extern "C" cpp::byte __heap_base __attribute__((weak));
extern "C" cpp::byte __heap_end __attribute__((weak));
#else
extern "C" cpp::byte _end;
extern "C" cpp::byte __llvm_libc_heap_limit;
#endif

using cpp::optional;
using cpp::span;

LIBC_INLINE constexpr bool IsPow2(size_t x) { return x && (x & (x - 1)) == 0; }

class FreeListHeap {
public:
#if defined(__wasi__)
  constexpr FreeListHeap() : begin(&__heap_base), end(&__heap_end) {}
#else
  constexpr FreeListHeap() : begin(&_end), end(&__llvm_libc_heap_limit) {}
#endif

  constexpr FreeListHeap(span<cpp::byte> region)
      : begin(region.begin()), end(region.end()) {}

  void *allocate(size_t size);
  void *aligned_allocate(size_t alignment, size_t size);
  // NOTE: All pointers passed to free must come from one of the other
  // allocation functions: `allocate`, `aligned_allocate`, `realloc`, `calloc`.
  void free(void *ptr);
  void *realloc(void *ptr, size_t size);
  void *calloc(size_t num, size_t size);

  cpp::span<cpp::byte> region() const { return {begin, end}; }

private:
  void init();

  void *allocate_impl(size_t alignment, size_t size);

#if defined(__wasi__)
  // Grow the linear memory to extend the heap by at least `size` bytes.
  bool grow(size_t size);
#endif

  span<cpp::byte> block_to_span(BlockRef block) {
    return span<cpp::byte>(block.usable_space(), block.inner_size());
  }

  bool shrink_in_place(BlockRef block, size_t size);

  bool is_valid_ptr(void *ptr) { return ptr >= begin && ptr < end; }

  cpp::byte *begin;
  cpp::byte *end;
  bool is_initialized = false;
  FreeStore free_store;
};

template <size_t BUFF_SIZE> class FreeListHeapBuffer : public FreeListHeap {
public:
  constexpr FreeListHeapBuffer() : FreeListHeap{buffer}, buffer{} {}

private:
  cpp::byte buffer[BUFF_SIZE];
};

LIBC_INLINE void FreeListHeap::init() {
  LIBC_ASSERT(!is_initialized && "duplicate initialization");
#if defined(__wasi__)
  if (begin == nullptr) {
    // Shared (PIC) links do not define __heap_base/__heap_end. Bootstrap the
    // heap by growing the linear memory, mirroring wasi-libc's sbrk: the new
    // memory pages become the initial heap region, and grow() extends it.
    constexpr size_t PAGE_SIZE = 0x10000;
    constexpr long INITIAL_PAGES = 16;
    long prev_pages = __builtin_wasm_memory_grow(0, INITIAL_PAGES);
    if (prev_pages >= 0) {
      begin = reinterpret_cast<cpp::byte *>(static_cast<size_t>(prev_pages) *
                                            PAGE_SIZE);
      end = reinterpret_cast<cpp::byte *>(static_cast<size_t>(prev_pages +
                                                              INITIAL_PAGES) *
                                          PAGE_SIZE);
    }
  }
#endif
  auto result = BlockRef::init(region());
  BlockRef block = *result;
#if defined(__wasi__)
  // The heap can grow via memory.grow, so the trie must be able to track
  // blocks larger than the initial heap. Size the range for the largest
  // possible block instead.
  free_store.set_range({0, size_t{1} << (sizeof(size_t) * 8 - 1)});
#else
  free_store.set_range({0, cpp::bit_ceil(block.inner_size())});
#endif
  free_store.insert(block);
  is_initialized = true;
}

#if defined(__wasi__)
LIBC_INLINE bool FreeListHeap::grow(size_t size) {
  // Grow the linear memory by at least `size` bytes (rounded up to whole
  // wasm pages, with a minimum increment to amortize grow calls).
  constexpr size_t PAGE_SIZE = 0x10000;
  constexpr size_t MIN_GROW = 4 * PAGE_SIZE;
  size_t grow_size = size > MIN_GROW ? size : MIN_GROW;
  grow_size = ((grow_size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
  long prev_pages = __builtin_wasm_memory_grow(0, grow_size / PAGE_SIZE);
  if (prev_pages < 0)
    return false;

  cpp::byte *old_end = end;
  end = reinterpret_cast<cpp::byte *>(static_cast<size_t>(prev_pages) *
                                          PAGE_SIZE +
                                      grow_size);

  // The sentinel last block was located at the old end of the region.
  // Replace it with a free block covering the grown area, terminated by a
  // new sentinel at the new end.
  BlockRef old_last(old_end - BlockRef::HEADER_SIZE);
  cpp::byte *new_sentinel_ptr = end - BlockRef::HEADER_SIZE;
  size_t old_last_next = old_last.load_next();

  if (old_last_next & BlockRef::PREV_FREE_MASK) {
    // The block before the sentinel is free; merge the new area into it.
    BlockRef prev_free = old_last.prev_free();
    free_store.remove(prev_free);
    size_t offset = static_cast<size_t>(new_sentinel_ptr - prev_free.header_ptr);
    prev_free.store_next(offset | (prev_free.load_next() &
                                   BlockRef::PREV_FREE_MASK));
    free_store.insert(prev_free);
  } else {
    // Turn the old sentinel into a regular free block.
    size_t offset =
        static_cast<size_t>(new_sentinel_ptr - old_last.header_ptr);
    old_last.store_next(offset);
    free_store.insert(old_last);
  }

  // Write the new sentinel; the block before it is free.
  BlockRef new_last(new_sentinel_ptr);
  new_last.store_next(BlockRef::HEADER_SIZE | BlockRef::LAST_MASK |
                      BlockRef::PREV_FREE_MASK);
  return true;
}
#endif // defined(__wasi__)

LIBC_INLINE void *FreeListHeap::allocate_impl(size_t alignment, size_t size) {
  if (size == 0)
    return nullptr;

  if (!is_initialized)
    init();

  size_t request_size = BlockRef::min_size_for_allocation(alignment, size);
  if (!request_size)
    return nullptr;

  BlockRef block = free_store.remove_best_fit(request_size);
#if defined(__wasi__)
  if (!block && grow(request_size + BlockRef::HEADER_SIZE))
    block = free_store.remove_best_fit(request_size);
#endif
  if (!block)
    return nullptr;

  auto block_info = BlockRef::allocate(block, alignment, size);
  if (block_info.next)
    free_store.insert(block_info.next);
  if (block_info.prev)
    free_store.insert(block_info.prev);

  block_info.block.mark_used();
  return block_info.block.usable_space();
}

LIBC_INLINE void *FreeListHeap::allocate(size_t size) {
  return allocate_impl(BlockRef::MIN_ALIGN, size);
}

LIBC_INLINE void *FreeListHeap::aligned_allocate(size_t alignment,
                                                 size_t size) {
  // The alignment must be an integral power of two.
  if (!IsPow2(alignment))
    return nullptr;

  // The size parameter must be an integral multiple of alignment.
  if (size % alignment != 0)
    return nullptr;

  // The minimum alignment supported by BlockRef is MIN_ALIGN.
  alignment = cpp::max(alignment, BlockRef::MIN_ALIGN);

  return allocate_impl(alignment, size);
}

LIBC_INLINE void FreeListHeap::free(void *ptr) {
  if (ptr == nullptr)
    return;

  cpp::byte *bytes = static_cast<cpp::byte *>(ptr);

  LIBC_ASSERT(is_valid_ptr(bytes) && "Invalid pointer");

  BlockRef block = BlockRef::from_usable_space(bytes);
  LIBC_ASSERT(block.next() && "sentinel last block cannot be freed");
  LIBC_ASSERT(block.used() && "double free");
  block.mark_free();

  // Can we combine with the left or right blocks?
  BlockRef prev_free = block.prev_free();
  BlockRef next = block.next();

  if (prev_free) {
    // Remove from free store and merge.
    free_store.remove(prev_free);
    block = prev_free;
    block.merge_next();
  }
  if (!next.used()) {
    free_store.remove(next);
    block.merge_next();
  }
  // Add back to the freelist
  free_store.insert(block);
}

LIBC_INLINE bool FreeListHeap::shrink_in_place(BlockRef block, size_t size) {
  size_t min_outer_size = BlockRef::outer_size(cpp::max(size, sizeof(size_t)));
  uintptr_t next_block_start = BlockRef::next_possible_block_start(
      block.addr() + min_outer_size, BlockRef::MIN_ALIGN);
  size_t new_outer_size = next_block_start - block.addr();
  if (block.outer_size() >= new_outer_size) {
    optional<BlockRef> next = block.split(size);
    // register the new block on successful split
    if (next.has_value()) {
      BlockRef next_block = *next;
      BlockRef right = next_block.next();
      // Since the original block was not the last block (the sentinel last
      // block is never split), the split-off remainder block `next_block` is
      // also not the last block. Thus, its next block `right` is guaranteed
      // to be non-null.
      LIBC_ASSERT(right && "right block must be non-null");
      if (!right.used()) {
        free_store.remove(right);
        next_block.merge_next();
      }
      free_store.insert(next_block);
    }
    return true;
  }
  return false;
}

// Follows constract of the C standard realloc() function
// If ptr is free'd, will return nullptr.
LIBC_INLINE void *FreeListHeap::realloc(void *ptr, size_t size) {
  if (size == 0) {
    free(ptr);
    return nullptr;
  }

  // If the pointer is nullptr, allocate a new memory.
  if (ptr == nullptr)
    return allocate(size);

  cpp::byte *bytes = static_cast<cpp::byte *>(ptr);

  if (!is_valid_ptr(bytes))
    return nullptr;

  BlockRef block = BlockRef::from_usable_space(bytes);
  if (!block.used())
    return nullptr;
  size_t old_size = block.inner_size();

  if (old_size >= size) {
    shrink_in_place(block, size);
    return ptr;
  }

  void *new_ptr = allocate(size);
  // Don't invalidate ptr if allocate(size) fails to initilize the memory.
  if (new_ptr == nullptr)
    return nullptr;
  LIBC_NAMESPACE::inline_memcpy(new_ptr, ptr, old_size);

  free(ptr);
  return new_ptr;
}

LIBC_INLINE void *FreeListHeap::calloc(size_t num, size_t size) {
  size_t bytes;
  if (__builtin_mul_overflow(num, size, &bytes))
    return nullptr;
  void *ptr = allocate(bytes);
  if (ptr != nullptr)
    LIBC_NAMESPACE::inline_memset(ptr, 0, bytes);
  return ptr;
}

extern FreeListHeap *freelist_heap;

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_FREELIST_HEAP_H
