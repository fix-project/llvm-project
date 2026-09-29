//===-- Internal state for the WASI mmap emulation --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_MMAN_WASI_MMAN_EMULATION_H
#define LLVM_LIBC_SRC_SYS_MMAN_WASI_MMAN_EMULATION_H

#include "hdr/types/off_t.h"
#include "hdr/types/size_t.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace mman_wasi {

struct Mapping {
  void *addr;
  size_t size;
  int fd;
  off_t offset;
  bool file_backed;
  bool shared;
  bool writable;
};

// The table of live emulated mappings.  The caller must hold the mman lock
// when mutating it.
Mapping *mapping_table();
unsigned &mapping_capacity();
Mapping *find_mapping(void *addr);
Mapping *reserve_mapping();
void release_mapping(Mapping *m);

void lock_mman();
void unlock_mman();

// Writes the contents of a shared file-backed mapping back to its file.
bool flush_mapping(const Mapping &m);

} // namespace mman_wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SYS_MMAN_WASI_MMAN_EMULATION_H
