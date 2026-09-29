//===-- WASI path resolution helpers --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Helpers which translate POSIX style paths into (dirfd, relative path)
/// pairs understood by the WASI `path_*` syscalls. Preopened directories
/// are discovered via `fd_prestat_get`/`fd_prestat_dir_name` and the
/// process working directory is emulated with a global string since WASI
/// preview1 has no `chdir` syscall.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_PATH_H
#define LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_PATH_H

#include "hdr/limits_macros.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/error_or.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// A resolved path: `path` is relative to the directory referred to by `dirfd`.
struct ResolvedPath {
  __wasi_fd_t dirfd;
  const char *path;
};

// Maximum path length supported by the resolver.
constexpr size_t PATH_MAX_SIZE = 4096;

// Resolve a possibly absolute POSIX path into a (dirfd, relative path) pair.
// `buf` (of at least PATH_MAX bytes) is used to build the normalized
// absolute path; the returned `path` points into it. Returns ENOENT if no
// preopened directory covers the path, EINVAL for null paths and
// ENAMETOOLONG if the path is too long.
ErrorOr<ResolvedPath> resolve_path(const char *path, char *buf,
                                   size_t buf_size);

// Resolve every component, including the final symlink when requested, and
// copy the absolute path into `buf`. Returns an error number on failure.
int canonicalize_path(const char *path, char *buf, size_t buf_size,
                      bool follow_final);

// Resolve a POSIX `*at()` style path. If `path` is absolute or `dirfd` is
// AT_FDCWD, the path is resolved against the emulated working directory
// (the returned `path` points into `buf`). Otherwise the pair
// (dirfd, path) is returned as-is for direct use with the WASI `path_*`
// syscalls.
ErrorOr<ResolvedPath> resolve_at(int dirfd, const char *path, char *buf,
                                 size_t buf_size);

// Change the emulated working directory. The path must resolve to an
// existing directory. Returns 0 on success or an error number on failure.
int chdir(const char *path);

// Copy the current emulated working directory into `buf` (at most `size`
// bytes including the null terminator). Returns 0 on success or an error
// number on failure.
int getcwd(char *buf, size_t size);

// Enumerate the visible child names contributed by preopened mounts below an
// absolute directory path. Pass 3 initially and the returned fd on subsequent
// calls; -1 means there are no more children.
int next_mount_child(const char *directory, int start_fd, char *name,
                     size_t name_size);
bool is_mount_child(const char *directory, const char *name, size_t name_len);
bool is_mount_directory(const char *directory);

// Registry mapping open directory fds to their resolved absolute paths,
// used to implement fchdir(). `register_fd_path` records the normalized
// absolute path for `fd`; `unregister_fd_path` removes it (on close);
// `lookup_fd_path` copies the recorded path into `buf`.
void register_fd_path(int fd, const char *abspath);
void unregister_fd_path(int fd);
bool lookup_fd_path(int fd, char *buf, size_t size);

// Extended registry also recording the POSIX open flags so that dup() can
// re-open the underlying path. `register_fd` records path and flags;
// `fd_dup_info` retrieves them; `move_fd_registration` transfers the
// registration from one fd number to another (after fd_renumber).
void register_fd(int fd, const char *abspath, int flags);
bool fd_dup_info(int fd, char *buf, size_t size, int *flags);
void move_fd_registration(int from_fd, int to_fd);

// WASI preview1 runtimes ignore the requested rights, so O_PATH descriptors
// cannot be identified via fd_fdstat_get. Track them explicitly instead.
void mark_fd_o_path(int fd);
void unmark_fd_o_path(int fd);
bool fd_is_o_path(int fd);

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_PATH_H
