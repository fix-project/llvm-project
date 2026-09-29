//===-- WASI path resolution implementation -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "path.h"

#include "hdr/fcntl_macros.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

namespace {

size_t str_len(const char *s) {
  size_t n = 0;
  while (s[n] != '\0')
    ++n;
  return n;
}

bool mem_eq(const char *a, const char *b, size_t n) {
  for (size_t i = 0; i < n; ++i)
    if (a[i] != b[i])
      return false;
  return true;
}

// The emulated current working directory. Only used when relative paths are
// passed to entrypoints; it is always an absolute path starting with '/'.
// NOTE: single-threaded only (LIBC_THREAD_MODE_IS_SINGLE).
char cwd_storage[PATH_MAX_SIZE] = "/";
size_t cwd_len = 1;

// Information about one preopened directory.
struct Preopen {
  __wasi_fd_t fd;
  char name[PATH_MAX_SIZE];
  size_t name_len;
};

// Iterate over the preopens starting at `start_fd`. Returns the next fd to
// query, or -1 when the end of the table is reached.
int next_preopen(int start_fd, Preopen &out) {
  for (int fd = start_fd;; ++fd) {
    __wasi_prestat_t prestat;
    __wasi_errno_t err =
        __wasi_fd_prestat_get(static_cast<__wasi_fd_t>(fd), &prestat);
    if (err != __WASI_ERRNO_SUCCESS)
      return -1;
    if (prestat.pr_type != __WASI_PREOPENTYPE_DIR)
      continue;
    if (prestat.pr_name_len >= PATH_MAX_SIZE)
      continue;
    if (__wasi_fd_prestat_dir_name(static_cast<__wasi_fd_t>(fd), out.name,
                                   prestat.pr_name_len) != __WASI_ERRNO_SUCCESS)
      continue;
    out.name[prestat.pr_name_len] = '\0';
    out.name_len = prestat.pr_name_len;
    out.fd = static_cast<__wasi_fd_t>(fd);
    return fd + 1;
  }
}

// Returns true if the preopen name denotes the root of the working
// directory namespace (e.g. "." or "/" or "").
bool is_root_preopen(const char *name, size_t len) {
  if (len == 0)
    return true;
  if (len == 1 && (name[0] == '/' || name[0] == '.'))
    return true;
  return false;
}

bool mount_child_of(const Preopen &po, const char *directory, char *name,
                    size_t name_size) {
  const char *p = po.name;
  size_t len = po.name_len;
  if (len >= 2 && p[0] == '.' && p[1] == '/') {
    p += 2;
    len -= 2;
  }
  while (len > 1 && p[len - 1] == '/')
    --len;
  if (is_root_preopen(p, len))
    return false;

  char absolute[PATH_MAX_SIZE];
  size_t n = 0;
  if (p[0] != '/')
    absolute[n++] = '/';
  if (n + len >= sizeof(absolute))
    return false;
  for (size_t i = 0; i < len; ++i)
    absolute[n++] = p[i];
  absolute[n] = '\0';

  size_t parent_len = str_len(directory);
  size_t child_start = parent_len == 1 && directory[0] == '/'
                           ? 1
                           : parent_len + 1;
  if (child_start >= n ||
      (parent_len != 1 &&
       (!mem_eq(absolute, directory, parent_len) ||
        absolute[parent_len] != '/')))
    return false;
  size_t child_end = child_start;
  while (child_end < n && absolute[child_end] != '/')
    ++child_end;
  size_t child_len = child_end - child_start;
  if (child_len == 0 || child_len + 1 > name_size)
    return false;
  for (size_t i = 0; i < child_len; ++i)
    name[i] = absolute[child_start + i];
  name[child_len] = '\0';
  return true;
}

} // namespace

int next_mount_child(const char *directory, int start_fd, char *name,
                     size_t name_size) {
  Preopen po;
  for (int next = next_preopen(start_fd, po); next >= 0;
       next = next_preopen(next, po)) {
    if (!mount_child_of(po, directory, name, name_size))
      continue;
    // Nested preopens may contribute the same ancestor. Emit it once.
    Preopen earlier;
    char candidate[PATH_MAX_SIZE];
    bool duplicate = false;
    for (int prev = next_preopen(3, earlier);
         prev >= 0 && earlier.fd < po.fd;
         prev = next_preopen(prev, earlier)) {
      if (mount_child_of(earlier, directory, candidate,
                         sizeof(candidate)) &&
          str_len(candidate) == str_len(name) &&
          mem_eq(candidate, name, str_len(name))) {
        duplicate = true;
        break;
      }
    }
    if (!duplicate)
      return next;
  }
  return -1;
}

bool is_mount_child(const char *directory, const char *name, size_t name_len) {
  char child[PATH_MAX_SIZE];
  for (int next = next_mount_child(directory, 3, child, sizeof(child));
       next >= 0;
       next = next_mount_child(directory, next, child, sizeof(child))) {
    if (str_len(child) == name_len && mem_eq(child, name, name_len))
      return true;
  }
  return false;
}

bool is_mount_directory(const char *directory) {
  char child[PATH_MAX_SIZE];
  return next_mount_child(directory, 3, child, sizeof(child)) >= 0;
}

namespace {

ErrorOr<ResolvedPath> route_path(char *buf, size_t buf_size) {
  size_t off = str_len(buf);
  // Find the longest preopened directory that covers the absolute path.
  __wasi_fd_t best_fd = -1;
  size_t best_len = 0;
  bool best_is_root = false;
  Preopen po;
  for (int next = next_preopen(3, po); next >= 0;
       next = next_preopen(next, po)) {
    const char *pname = po.name;
    size_t plen = po.name_len;
    // Normalize: "./x" -> "/x", "." -> "/", strip trailing '/'.
    if (plen >= 2 && pname[0] == '.' && pname[1] == '/') {
      pname += 2;
      plen -= 2;
    }
    while (plen > 1 && pname[plen - 1] == '/')
      --plen;

    bool root = is_root_preopen(pname, plen);
    size_t match_len = 0;
    bool matches = false;
    if (root) {
      // Matches any absolute path; the remainder is the path minus '/'.
      matches = off >= 1 && buf[0] == '/';
      match_len = 0;
    } else {
      if (plen + 1 > buf_size)
        continue;
      // Preopen names are absolute in practice; prepend '/' if missing.
      char norm[PATH_MAX_SIZE];
      size_t nlen = 0;
      if (pname[0] == '/') {
        if (plen >= buf_size)
          continue;
        for (nlen = 0; nlen < plen; ++nlen)
          norm[nlen] = pname[nlen];
      } else {
        if (plen + 1 >= buf_size)
          continue;
        norm[0] = '/';
        for (nlen = 1; nlen <= plen; ++nlen)
          norm[nlen] = pname[nlen - 1];
      }
      norm[nlen] = '\0';
      if (off >= nlen && mem_eq(buf, norm, nlen)) {
        if (off == nlen || buf[nlen] == '/') {
          matches = true;
          match_len = nlen;
        }
      }
    }
    if (!matches)
      continue;
    if (best_fd == -1 || match_len > best_len ||
        (match_len == best_len && root && !best_is_root)) {
      best_fd = po.fd;
      best_len = match_len;
      best_is_root = root;
    }
  }
  if (best_fd == -1)
    return Error(ENOENT);

  const char *rel = buf + best_len;
  while (*rel == '/')
    ++rel;
  if (*rel == '\0')
    return ResolvedPath{best_fd, "."};
  return ResolvedPath{best_fd, rel};
}

} // namespace

int canonicalize_path(const char *path, char *buf, size_t buf_size,
                      bool follow_final) {
  if (path == nullptr)
    return EINVAL;
  if (path[0] == '\0')
    return ENOENT;
  if (buf_size < 2)
    return ENAMETOOLONG;
  size_t capacity = buf_size < PATH_MAX_SIZE ? buf_size : PATH_MAX_SIZE;

  char pending[PATH_MAX_SIZE];
  size_t pending_len = str_len(path);
  if (pending_len >= capacity)
    return ENAMETOOLONG;
  for (size_t i = 0; i <= pending_len; ++i)
    pending[i] = path[i];

  char current[PATH_MAX_SIZE];
  size_t current_len = 1;
  current[0] = '/';
  current[1] = '\0';
  if (path[0] != '/') {
    current_len = cwd_len;
    for (size_t i = 0; i <= current_len; ++i)
      current[i] = cwd_storage[i];
  }

  unsigned links = 0;
  size_t pos = 0;
  while (pos < pending_len) {
    while (pos < pending_len && pending[pos] == '/')
      ++pos;
    if (pos == pending_len)
      break;
    size_t start = pos;
    while (pos < pending_len && pending[pos] != '/')
      ++pos;
    size_t end = pos;
    while (pos < pending_len && pending[pos] == '/')
      ++pos;
    bool has_more = pos < pending_len;
    bool need_directory =
        has_more || (follow_final && pending[pending_len - 1] == '/');
    size_t len = end - start;
    if (len == 1 && pending[start] == '.')
      continue;
    if (len == 2 && pending[start] == '.' && pending[start + 1] == '.') {
      while (current_len > 1 && current[current_len - 1] != '/')
        --current_len;
      if (current_len > 1)
        --current_len;
      current[current_len] = '\0';
      continue;
    }

    size_t parent_len = current_len;
    if (current_len > 1)
      current[current_len++] = '/';
    if (len >= capacity - current_len)
      return ENAMETOOLONG;
    for (size_t i = 0; i < len; ++i)
      current[current_len++] = pending[start + i];
    current[current_len] = '\0';

    if (!need_directory && !follow_final)
      continue;
    if (is_mount_directory(current))
      continue;
    auto resolved = route_path(current, capacity);
    if (!resolved.has_value())
      return resolved.error();
    __wasi_filestat_t st;
    __wasi_errno_t err = __wasi_path_filestat_get(
        resolved->dirfd, 0, resolved->path, str_len(resolved->path), &st);
    if (err != __WASI_ERRNO_SUCCESS)
      return wasi_to_errno(err);
    if (st.st_filetype == __WASI_FILETYPE_SYMBOLIC_LINK) {
      if (++links > 40)
        return ELOOP;
      char target[PATH_MAX_SIZE];
      __wasi_size_t target_len;
      err = __wasi_path_readlink(
          resolved->dirfd, resolved->path, str_len(resolved->path), target,
          static_cast<__wasi_size_t>(capacity), &target_len);
      if (err != __WASI_ERRNO_SUCCESS)
        return wasi_to_errno(err);
      if (target_len == 0)
        return ENOENT;
      if (target_len >= capacity ||
          target_len + (pending_len - end) >= capacity)
        return ENAMETOOLONG;
      char next[PATH_MAX_SIZE];
      for (size_t i = 0; i < target_len; ++i)
        next[i] = target[i];
      for (size_t i = end; i <= pending_len; ++i)
        next[target_len + i - end] = pending[i];
      pending_len = target_len + pending_len - end;
      for (size_t i = 0; i <= pending_len; ++i)
        pending[i] = next[i];
      current_len = target[0] == '/' ? 1 : parent_len;
      current[current_len] = '\0';
      pos = 0;
    } else if (need_directory && st.st_filetype != __WASI_FILETYPE_DIRECTORY) {
      return ENOTDIR;
    }
  }

  if (current_len >= capacity)
    return ENAMETOOLONG;
  for (size_t i = 0; i <= current_len; ++i)
    buf[i] = current[i];
  return 0;
}

ErrorOr<ResolvedPath> resolve_path(const char *path, char *buf,
                                   size_t buf_size) {
  int err = canonicalize_path(path, buf, buf_size, false);
  if (err != 0)
    return Error(err);
  return route_path(buf, buf_size);
}

ErrorOr<ResolvedPath> resolve_at(int dirfd, const char *path, char *buf,
                                 size_t buf_size) {
  if (path == nullptr)
    return Error(EINVAL);
  if (path[0] == '/' || dirfd == AT_FDCWD)
    return resolve_path(path, buf, buf_size);
  if (dirfd < 0)
    return Error(EBADF);
  if (path[0] == '\0')
    return Error(ENOENT);
  // A known directory fd participates in the mounted namespace. Resolve its
  // child through the normal router so an openat() can cross into a nested
  // preopen, and so callers can record the resulting absolute path.
  if (buf_size != 0) {
    buf[0] = '\0';
    char parent[PATH_MAX_SIZE];
    if (lookup_fd_path(dirfd, parent, sizeof(parent))) {
      size_t parent_len = str_len(parent);
      size_t path_len = str_len(path);
      bool separator = parent_len != 1 || parent[0] != '/';
      if (parent_len + separator + path_len < PATH_MAX_SIZE) {
        char combined[PATH_MAX_SIZE];
        for (size_t i = 0; i < parent_len; ++i)
          combined[i] = parent[i];
        if (separator)
          combined[parent_len++] = '/';
        for (size_t i = 0; i <= path_len; ++i)
          combined[parent_len + i] = path[i];
        return resolve_path(combined, buf, buf_size);
      }
      return Error(ENAMETOOLONG);
    }
  }
  return ResolvedPath{static_cast<__wasi_fd_t>(dirfd), path};
}

int chdir(const char *path) {
  if (path == nullptr)
    return EINVAL;
  char buf[PATH_MAX_SIZE];
  int canonical_err = canonicalize_path(path, buf, sizeof(buf), true);
  if (canonical_err != 0)
    return canonical_err;
  if (is_mount_directory(buf)) {
    size_t len = str_len(buf);
    for (size_t i = 0; i <= len; ++i)
      cwd_storage[i] = buf[i];
    cwd_len = len;
    return 0;
  }
  auto resolved = route_path(buf, sizeof(buf));
  if (!resolved.has_value())
    return resolved.error();

  // Verify the path refers to a directory by opening it.
  __wasi_fd_t fd;
  __wasi_errno_t err = __wasi_path_open(
      resolved->dirfd, __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      str_len(resolved->path), 0, __WASI_RIGHT_FD_FILESTAT_GET, 0, 0, &fd);
  if (err != __WASI_ERRNO_SUCCESS)
    return wasi_to_errno(err);
  __wasi_filestat_t st;
  err = __wasi_fd_filestat_get(fd, &st);
  __wasi_fd_close(fd);
  if (err != __WASI_ERRNO_SUCCESS)
    return wasi_to_errno(err);
  if (st.st_filetype != __WASI_FILETYPE_DIRECTORY)
    return ENOTDIR;

  // Store the normalized absolute path (buf holds it already).
  size_t len = str_len(buf);
  if (len >= PATH_MAX_SIZE)
    return ENAMETOOLONG;
  for (size_t i = 0; i <= len; ++i)
    cwd_storage[i] = buf[i];
  cwd_len = len;
  return 0;
}

int getcwd(char *buf, size_t size) {
  if (buf == nullptr)
    return EINVAL;
  if (size < cwd_len + 1)
    return ERANGE;
  for (size_t i = 0; i <= cwd_len; ++i)
    buf[i] = cwd_storage[i];
  return 0;
}

namespace {
struct FdPath {
  int fd;
  int flags;
  bool used;
  char path[PATH_MAX_SIZE];
};
constexpr size_t FD_PATH_TABLE_SIZE = 64;
FdPath fd_path_table[FD_PATH_TABLE_SIZE];
} // namespace

void register_fd(int fd, const char *abspath, int flags) {
  size_t len = str_len(abspath);
  if (len >= PATH_MAX_SIZE)
    return;
  FdPath *free_slot = nullptr;
  for (size_t i = 0; i < FD_PATH_TABLE_SIZE; ++i) {
    if (fd_path_table[i].used && fd_path_table[i].fd == fd) {
      free_slot = &fd_path_table[i];
      break;
    }
    if (!fd_path_table[i].used && free_slot == nullptr)
      free_slot = &fd_path_table[i];
  }
  if (free_slot == nullptr)
    return;
  free_slot->fd = fd;
  free_slot->flags = flags;
  free_slot->used = true;
  for (size_t i = 0; i <= len; ++i)
    free_slot->path[i] = abspath[i];
}

void register_fd_path(int fd, const char *abspath) {
  register_fd(fd, abspath, 0);
}

void unregister_fd_path(int fd) {
  for (size_t i = 0; i < FD_PATH_TABLE_SIZE; ++i) {
    if (fd_path_table[i].used && fd_path_table[i].fd == fd) {
      fd_path_table[i].used = false;
      return;
    }
  }
}

namespace {
constexpr size_t O_PATH_TABLE_SIZE = 64;
int o_path_fds[O_PATH_TABLE_SIZE];
} // namespace

void mark_fd_o_path(int fd) {
  for (size_t i = 0; i < O_PATH_TABLE_SIZE; ++i) {
    if (o_path_fds[i] == fd)
      return;
    if (o_path_fds[i] == 0) {
      o_path_fds[i] = fd;
      return;
    }
  }
}

void unmark_fd_o_path(int fd) {
  for (size_t i = 0; i < O_PATH_TABLE_SIZE; ++i)
    if (o_path_fds[i] == fd)
      o_path_fds[i] = 0;
}

bool fd_is_o_path(int fd) {
  for (size_t i = 0; i < O_PATH_TABLE_SIZE; ++i)
    if (o_path_fds[i] == fd)
      return true;
  return false;
}

bool lookup_fd_path(int fd, char *buf, size_t size) {
  for (size_t i = 0; i < FD_PATH_TABLE_SIZE; ++i) {
    if (fd_path_table[i].used && fd_path_table[i].fd == fd) {
      size_t len = str_len(fd_path_table[i].path);
      if (len + 1 > size)
        return false;
      for (size_t k = 0; k <= len; ++k)
        buf[k] = fd_path_table[i].path[k];
      return true;
    }
  }
  return false;
}

bool fd_dup_info(int fd, char *buf, size_t size, int *flags) {
  for (size_t i = 0; i < FD_PATH_TABLE_SIZE; ++i) {
    if (fd_path_table[i].used && fd_path_table[i].fd == fd) {
      size_t len = str_len(fd_path_table[i].path);
      if (len + 1 > size)
        return false;
      for (size_t k = 0; k <= len; ++k)
        buf[k] = fd_path_table[i].path[k];
      if (flags != nullptr)
        *flags = fd_path_table[i].flags;
      return true;
    }
  }
  return false;
}

void move_fd_registration(int from_fd, int to_fd) {
  FdPath *entry = nullptr;
  for (size_t i = 0; i < FD_PATH_TABLE_SIZE; ++i) {
    if (fd_path_table[i].used && fd_path_table[i].fd == from_fd)
      entry = &fd_path_table[i];
    else if (fd_path_table[i].used && fd_path_table[i].fd == to_fd)
      fd_path_table[i].used = false;
  }
  if (entry != nullptr)
    entry->fd = to_fd;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL
