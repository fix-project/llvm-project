//===-- WASI mmap emulation -------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/mmap.h"
#include "src/sys/mman/wasi/mman_emulation.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/fcntl/wasi/open_utils.h"

#include "hdr/fcntl_macros.h"

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {
namespace mman_wasi {

namespace {

constexpr unsigned kDefaultCapacity = 1024;

struct MmanState {
  Mapping *table;
  unsigned capacity;
  unsigned *free_list;
  unsigned free_count;
  unsigned free_capacity;
  int lock;
};

MmanState g_state = {nullptr, 0, nullptr, 0, 0, 0};

void ensure_table() {
  if (g_state.table != nullptr)
    return;
  unsigned cap = kDefaultCapacity;
  Mapping *table =
      static_cast<Mapping *>(calloc(cap, sizeof(Mapping) + sizeof(unsigned)));
  if (table == nullptr)
    return;
  unsigned *free_list = reinterpret_cast<unsigned *>(table + cap);
  for (unsigned i = 0; i < cap; ++i) {
    table[i].addr = nullptr;
    free_list[i] = cap - 1 - i;
  }
  g_state.table = table;
  g_state.capacity = cap;
  g_state.free_list = free_list;
  g_state.free_count = cap;
  g_state.free_capacity = cap;
}

bool grow_table() {
  unsigned old_cap = g_state.capacity;
  unsigned new_cap = old_cap == 0 ? kDefaultCapacity : old_cap * 2;
  if (new_cap <= old_cap)
    return false;
  Mapping *new_table = static_cast<Mapping *>(
      calloc(new_cap, sizeof(Mapping) + sizeof(unsigned)));
  if (new_table == nullptr)
    return false;
  unsigned *new_free = reinterpret_cast<unsigned *>(new_table + new_cap);
  unsigned used = 0;
  // Copy live mappings, compacting them to the front of the new table.
  for (unsigned i = 0; i < old_cap; ++i) {
    if (g_state.table[i].addr != nullptr)
      new_table[used++] = g_state.table[i];
  }
  // Every remaining slot is free; list them so reserve_mapping can pop them.
  unsigned fc = 0;
  for (unsigned i = new_cap; i-- > used;)
    new_free[fc++] = i;
  free(g_state.table);
  g_state.table = new_table;
  g_state.capacity = new_cap;
  g_state.free_list = new_free;
  g_state.free_count = fc;
  g_state.free_capacity = new_cap;
  return true;
}

} // namespace

Mapping *mapping_table() {
  ensure_table();
  return g_state.table;
}

unsigned &mapping_capacity() {
  ensure_table();
  return g_state.capacity;
}

Mapping *find_mapping(void *addr) {
  Mapping *table = mapping_table();
  unsigned cap = mapping_capacity();
  for (unsigned i = 0; i < cap; ++i) {
    if (table[i].addr == addr && addr != nullptr)
      return &table[i];
  }
  return nullptr;
}

Mapping *reserve_mapping() {
  ensure_table();
  if (g_state.table == nullptr)
    return nullptr;
  if (g_state.free_count == 0 && !grow_table())
    return nullptr;
  if (g_state.free_count == 0)
    return nullptr;
  unsigned idx = g_state.free_list[--g_state.free_count];
  Mapping *m = &g_state.table[idx];
  m->addr = nullptr;
  m->file_backed = false;
  m->shared = false;
  return m;
}

void release_mapping(Mapping *m) {
  if (m == nullptr || m->addr == nullptr)
    return;
  unsigned idx = static_cast<unsigned>(m - g_state.table);
  m->addr = nullptr;
  if (g_state.free_count < g_state.free_capacity)
    g_state.free_list[g_state.free_count++] = idx;
}

void lock_mman() {
  ensure_table();
  while (__atomic_exchange_n(&g_state.lock, 1, __ATOMIC_ACQUIRE)) {
    // Spin; WASI targets are effectively single-threaded for now.
  }
}

void unlock_mman() { __atomic_store_n(&g_state.lock, 0, __ATOMIC_RELEASE); }

bool flush_mapping_range(const Mapping &m, size_t begin, size_t size) {
  if (!m.file_backed || !m.shared || !m.writable)
    return true;
  const char *p = static_cast<const char *>(m.addr) + begin;
  size_t remaining = size;
  off_t offset = m.offset + static_cast<off_t>(begin);
  while (remaining > 0) {
    wasi::__wasi_size_t chunk = static_cast<wasi::__wasi_size_t>(
        remaining > 0x40000000u ? 0x40000000u : remaining);
    wasi::__wasi_ciovec_t iov = {p, chunk};
    wasi::__wasi_size_t written = 0;
    wasi::__wasi_errno_t err = wasi::__wasi_fd_pwrite(
        static_cast<wasi::__wasi_fd_t>(m.fd), &iov, 1,
        static_cast<wasi::__wasi_filesize_t>(offset), &written);
    if (err != wasi::__WASI_ERRNO_SUCCESS || written == 0)
      return false;
    p += written;
    offset += static_cast<off_t>(written);
    remaining -= written;
  }
  return true;
}

bool flush_mapping(const Mapping &m) {
  return flush_mapping_range(m, 0, m.size);
}

void close_retained_fd(int fd) {
  wasi::unregister_fd_path(fd);
  wasi::unmark_fd_o_path(fd);
  wasi::__wasi_fd_close(static_cast<wasi::__wasi_fd_t>(fd));
}

// WASI preview1 cannot duplicate an fd. Reopen the recorded path while the
// caller's fd is live, then verify that it still names the same file. Keep
// this independent fd until the mapping is released.
int retain_file_fd(int fd, const wasi::__wasi_filestat_t &original) {
  if (original.st_ino == 0)
    return -ENOTSUP;
  char path[wasi::PATH_MAX_SIZE];
  int flags = 0;
  if (!wasi::fd_dup_info(fd, path, sizeof(path), &flags))
    return -ENOTSUP;
  flags &= ~(O_CREAT | O_EXCL | O_TRUNC | O_APPEND);
  int retained = wasi::openat_impl(AT_FDCWD, path, flags, 0);
  if (retained < 0)
    return retained;

  wasi::__wasi_filestat_t current;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_filestat_get(retained, &current);
  if (err != wasi::__WASI_ERRNO_SUCCESS ||
      current.st_dev != original.st_dev || current.st_ino != original.st_ino) {
    close_retained_fd(retained);
    return err == wasi::__WASI_ERRNO_SUCCESS ? -ENOTSUP
                                            : -wasi::wasi_to_errno(err);
  }
  return retained;
}

} // namespace mman_wasi

LLVM_LIBC_FUNCTION(void *, mmap,
                   (void *addr, size_t size, int prot, int flags, int fd,
                    off_t offset)) {
  (void)addr;
  if (size == 0) {
    libc_errno = EINVAL;
    return MAP_FAILED;
  }
  if (flags & MAP_FIXED) {
    libc_errno = ENODEV;
    return MAP_FAILED;
  }
  if (!!(flags & MAP_PRIVATE) == !!(flags & MAP_SHARED) || offset < 0 ||
      (offset & 4095) != 0) {
    libc_errno = EINVAL;
    return MAP_FAILED;
  }

  // Round the request up to a page so that munmap() with the caller's
  // original size still matches the recorded mapping.
  constexpr size_t kPageSize = 4096;
  if (size > SIZE_MAX - (kPageSize - 1)) {
    libc_errno = ENOMEM;
    return MAP_FAILED;
  }
  size_t rounded = (size + kPageSize - 1) & ~(kPageSize - 1);

  bool anonymous = (flags & MAP_ANONYMOUS) != 0;
  bool retain_fd = !anonymous && (flags & MAP_SHARED) && (prot & PROT_WRITE);
  wasi::__wasi_filestat_t original = {};
  if (!anonymous && fd < 0) {
    libc_errno = EBADF;
    return MAP_FAILED;
  }
  if (retain_fd) {
    wasi::__wasi_fdstat_t fdstat;
    wasi::__wasi_errno_t err = wasi::__wasi_fd_fdstat_get(fd, &fdstat);
    if (err != wasi::__WASI_ERRNO_SUCCESS) {
      libc_errno = wasi::wasi_to_errno(err);
      return MAP_FAILED;
    }
    if ((fdstat.fs_rights_base & wasi::__WASI_RIGHT_FD_WRITE) == 0) {
      libc_errno = EACCES;
      return MAP_FAILED;
    }
    err = wasi::__wasi_fd_filestat_get(fd, &original);
    if (err != wasi::__WASI_ERRNO_SUCCESS) {
      libc_errno = wasi::wasi_to_errno(err);
      return MAP_FAILED;
    }
    if (static_cast<uint64_t>(offset) > original.st_size ||
        size > original.st_size - static_cast<uint64_t>(offset)) {
      libc_errno = ENOTSUP;
      return MAP_FAILED;
    }
  }

  void *buf = aligned_alloc(kPageSize, rounded);
  if (buf == nullptr) {
    libc_errno = ENOMEM;
    return MAP_FAILED;
  }
  memset(buf, 0, rounded);

  if (!anonymous) {
    char *p = static_cast<char *>(buf);
    size_t remaining = size;
    wasi::__wasi_filesize_t file_offset =
        static_cast<wasi::__wasi_filesize_t>(offset);
    while (remaining > 0) {
      wasi::__wasi_iovec_t iov = {
          p, static_cast<wasi::__wasi_size_t>(
                 remaining > 0x40000000u ? 0x40000000u : remaining)};
      wasi::__wasi_size_t nread = 0;
      wasi::__wasi_errno_t err = wasi::__wasi_fd_pread(
          static_cast<wasi::__wasi_fd_t>(fd), &iov, 1, file_offset, &nread);
      if (err == wasi::__WASI_ERRNO_SUCCESS && nread > 0) {
        p += nread;
        file_offset += nread;
        remaining -= nread;
        continue;
      }
      if (err != wasi::__WASI_ERRNO_SUCCESS) {
        free(buf);
        libc_errno = wasi::wasi_to_errno(err);
        return MAP_FAILED;
      }
      // EOF leaves the remainder zeroed.
      break;
    }
  }

  int mapping_fd = -1;
  if (retain_fd) {
    mapping_fd = mman_wasi::retain_file_fd(fd, original);
    if (mapping_fd < 0) {
      free(buf);
      libc_errno = -mapping_fd;
      return MAP_FAILED;
    }
  }

  mman_wasi::lock_mman();
  mman_wasi::Mapping *m = mman_wasi::reserve_mapping();
  if (m == nullptr) {
    mman_wasi::unlock_mman();
    if (mapping_fd >= 0)
      mman_wasi::close_retained_fd(mapping_fd);
    free(buf);
    libc_errno = ENOMEM;
    return MAP_FAILED;
  }
  m->addr = buf;
  m->size = size;
  m->fd = mapping_fd;
  m->offset = offset;
  m->file_backed = !anonymous;
  m->shared = (flags & MAP_SHARED) != 0;
  m->writable = (prot & PROT_WRITE) != 0;
  mman_wasi::unlock_mman();

  return buf;
}

} // namespace LIBC_NAMESPACE_DECL
