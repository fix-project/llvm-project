//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// WASI implementation of the Dir helpers.
///
//===----------------------------------------------------------------------===//

#include "src/__support/File/dir.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/error_or.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

using namespace wasi;

namespace {

size_t str_len(const char *s) {
  size_t n = 0;
  while (s[n] != '\0')
    ++n;
  return n;
}

// WASI's fd_readdir takes an explicit cookie (the offset of the next entry).
// The Dir class expects platform_fetch_dirents to advance the stream on
// every call, so we track the cookie per open directory fd here.
// NOTE: single-threaded only.
struct DirCookie {
  int fd = -1;
  wasi::__wasi_dircookie_t cookie = 0;
};
constexpr size_t MAX_OPEN_DIRS = 64;
DirCookie dir_cookies[MAX_OPEN_DIRS];

DirCookie &cookie_for_fd(int fd, bool create) {
  for (size_t i = 0; i < MAX_OPEN_DIRS; ++i)
    if (dir_cookies[i].fd == fd)
      return dir_cookies[i];
  if (!create) {
    // Should not happen; fall back to slot 0.
    return dir_cookies[0];
  }
  for (size_t i = 0; i < MAX_OPEN_DIRS; ++i) {
    if (dir_cookies[i].fd == -1) {
      dir_cookies[i].fd = fd;
      dir_cookies[i].cookie = 0;
      return dir_cookies[i];
    }
  }
  return dir_cookies[0];
}

} // namespace

ErrorOr<int> platform_opendir(const char *name) {
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(name, buf, sizeof(buf));
  if (!resolved.has_value())
    return Error(resolved.error());

  __wasi_fd_t fd;
  __wasi_errno_t err = wasi::__wasi_path_open(
      resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      static_cast<wasi::__wasi_size_t>(str_len(resolved->path)), 0,
      wasi::__WASI_RIGHT_FD_READDIR | wasi::__WASI_RIGHT_FD_FILESTAT_GET, 0, 0,
      &fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return Error(wasi::wasi_to_errno(err));

  // Verify that the opened descriptor refers to a directory.
  __wasi_fdstat_t fdstat;
  err = wasi::__wasi_fd_fdstat_get(fd, &fdstat);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    wasi::__wasi_fd_close(fd);
    return Error(wasi::wasi_to_errno(err));
  }
  if (fdstat.fs_filetype != wasi::__WASI_FILETYPE_DIRECTORY) {
    wasi::__wasi_fd_close(fd);
    return Error(ENOTDIR);
  }

  // Reset the cookie in case this fd number was reused previously.
  DirCookie &entry = cookie_for_fd(static_cast<int>(fd), true);
  entry.fd = static_cast<int>(fd);
  entry.cookie = 0;
  return static_cast<int>(fd);
}

// WASI fd_readdir records are packed: a 24-byte header (d_next, d_ino,
// d_namlen, d_type, 3 padding bytes) followed by d_namlen bytes of name.
// The names are not NUL-terminated and the last record in a buffer may be
// truncated.  The Dir class expects complete, NUL-terminated records, so
// raw records are read into scratch space and repacked into the caller's
// buffer as 8-byte-aligned records with a NUL terminator after the name.
// The scratch size is chosen so that the repacked output always fits in a
// Dir buffer of 4096 bytes: each record grows by at most 8 bytes and the
// smallest possible record is 25 bytes.
constexpr size_t RAW_BUF_SIZE = 3072;
alignas(uint64_t) uint8_t raw_dirent_buf[RAW_BUF_SIZE];

LIBC_INLINE size_t align_up(size_t n) { return (n + 7) & ~static_cast<size_t>(7); }

ErrorOr<size_t> platform_fetch_dirents(int fd, cpp::span<uint8_t> buffer) {
  DirCookie &entry = cookie_for_fd(fd, false);
  wasi::__wasi_size_t buf_used = 0;
  __wasi_errno_t err = wasi::__wasi_fd_readdir(
      static_cast<wasi::__wasi_fd_t>(fd), raw_dirent_buf, RAW_BUF_SIZE,
      entry.cookie, &buf_used);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return Error(wasi::wasi_to_errno(err));

  size_t in = 0;
  size_t out = 0;
  wasi::__wasi_dircookie_t next_cookie = entry.cookie;
  while (in + 24 <= buf_used) {
    const uint8_t *rec = raw_dirent_buf + in;
    wasi::__wasi_dircookie_t d_next;
    __builtin_memcpy(&d_next, rec, 8);
    wasi::__wasi_size_t d_namlen;
    __builtin_memcpy(&d_namlen, rec + 16, 4);
    size_t raw_reclen = 24 + d_namlen;
    // Stop at a truncated trailing record; it will be refetched with the
    // next call using the current cookie.
    if (in + raw_reclen > buf_used)
      break;
    size_t reclen = align_up(24 + d_namlen + 1);
    if (out + reclen > buffer.size())
      break;
    uint8_t *dst = buffer.data() + out;
    __builtin_memcpy(dst, rec, 20);
    // Clear the padding between d_type and d_name.
    dst[20] = rec[20];
    dst[21] = 0;
    dst[22] = 0;
    dst[23] = 0;
    __builtin_memcpy(dst + 24, rec + 24, d_namlen);
    dst[24 + d_namlen] = '\0';
    next_cookie = d_next;
    in += raw_reclen;
    out += reclen;
  }
  entry.cookie = next_cookie;
  return out;
}

int platform_closedir(int fd) {
  for (size_t i = 0; i < MAX_OPEN_DIRS; ++i)
    if (dir_cookies[i].fd == fd)
      dir_cookies[i].fd = -1;
  __wasi_errno_t err =
      wasi::__wasi_fd_close(static_cast<wasi::__wasi_fd_t>(fd));
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return wasi::wasi_to_errno(err);
  return 0;
}

int platform_check_dir(int fd) {
  __wasi_fdstat_t fdstat;
  __wasi_errno_t err = wasi::__wasi_fd_fdstat_get(
      static_cast<wasi::__wasi_fd_t>(fd), &fdstat);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return wasi::wasi_to_errno(err);
  if (fdstat.fs_filetype != wasi::__WASI_FILETYPE_DIRECTORY)
    return ENOTDIR;
  // An O_PATH descriptor cannot be used to read directory entries.
  if (wasi::fd_is_o_path(fd))
    return EBADF;
  return 0;
}

size_t platform_dir_reclen(struct dirent *d) {
  // Records are repacked by platform_fetch_dirents as 8-byte-aligned
  // records with a NUL terminator after the name.
  return align_up(24 + d->d_namlen + 1);
}

} // namespace LIBC_NAMESPACE_DECL
