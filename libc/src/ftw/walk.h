#ifndef LLVM_LIBC_SRC_FTW_WALK_H
#define LLVM_LIBC_SRC_FTW_WALK_H

#include <dirent.h>
#include <errno.h>
#include <ftw.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

namespace LIBC_NAMESPACE_DECL {
namespace ftw_internal {

struct Context {
  int (*simple)(const char *, const struct stat *, int);
  int (*extended)(const char *, const struct stat *, int, struct FTW *);
  int flags;
  dev_t root_device;
};

struct Name {
  Name *next;
  char text[1];
};

struct Ancestor {
  dev_t device;
  ino_t inode;
  const Ancestor *previous;
};

inline int callback(Context &ctx, const char *path, const struct stat *st,
                    int type, int level) {
  int base = 0;
  for (int i = 0; path[i]; ++i)
    if (path[i] == '/')
      base = i + 1;
  struct FTW info = {base, level};
  if (!(ctx.flags & FTW_CHDIR))
    return ctx.extended ? ctx.extended(path, st, type, &info)
                        : ctx.simple(path, st, type);

  char original[PATH_MAX];
  if (!getcwd(original, sizeof(original)))
    return -1;
  char *parent = static_cast<char *>(malloc(static_cast<size_t>(base) + 2));
  if (!parent) {
    errno = ENOMEM;
    return -1;
  }
  if (base == 0) {
    parent[0] = '.';
    parent[1] = '\0';
  } else {
    memcpy(parent, path, base);
    parent[base] = '\0';
  }
  int changed = chdir(parent);
  free(parent);
  if (changed != 0)
    return -1;
  int result = ctx.extended ? ctx.extended(path, st, type, &info)
                            : ctx.simple(path, st, type);
  int saved_errno = errno;
  if (chdir(original) != 0)
    return -1;
  errno = saved_errno;
  return result;
}

inline int walk(Context &ctx, const char *path, int level,
                const Ancestor *ancestor = nullptr) {
  struct stat st = {};
  int error = (ctx.flags & FTW_PHYS) ? lstat(path, &st) : stat(path, &st);
  if (error != 0) {
    if (!(ctx.flags & FTW_PHYS) && lstat(path, &st) == 0 && S_ISLNK(st.st_mode))
      return callback(ctx, path, &st, FTW_SLN, level);
    return callback(ctx, path, &st, FTW_NS, level);
  }
  if (level == 0)
    ctx.root_device = st.st_dev;
  if (S_ISLNK(st.st_mode))
    return callback(ctx, path, &st, FTW_SL, level);
  if (!S_ISDIR(st.st_mode))
    return callback(ctx, path, &st, FTW_F, level);

  for (const Ancestor *p = ancestor; p; p = p->previous)
    if (p->device == st.st_dev && p->inode == st.st_ino)
      return 0;
  Ancestor current = {st.st_dev, st.st_ino, ancestor};

  DIR *dir = opendir(path);
  if (!dir)
    return callback(ctx, path, &st, FTW_DNR, level);
  if (!(ctx.flags & FTW_DEPTH)) {
    int result = callback(ctx, path, &st, FTW_D, level);
    if (result != 0) {
      closedir(dir);
      return result;
    }
  }

  // Close this directory before descending so one descriptor suffices.
  Name *names = nullptr;
  int read_error = 0;
  for (;;) {
    errno = 0;
    struct dirent *entry = readdir(dir);
    if (!entry) {
      read_error = errno;
      break;
    }
    if ((entry->d_name[0] == '.' && entry->d_name[1] == '\0') ||
        (entry->d_name[0] == '.' && entry->d_name[1] == '.' &&
         entry->d_name[2] == '\0'))
      continue;
    size_t length = strlen(entry->d_name);
    Name *name = static_cast<Name *>(malloc(sizeof(Name) + length));
    if (!name) {
      read_error = ENOMEM;
      break;
    }
    memcpy(name->text, entry->d_name, length + 1);
    name->next = names;
    names = name;
  }
  closedir(dir);
  int result = 0;
  if (read_error) {
    errno = read_error;
    result = -1;
  }
  while (names) {
    Name *name = names;
    names = name->next;
    if (result == 0) {
      size_t length = strlen(path);
      size_t child_length = strlen(name->text);
      bool slash = length && path[length - 1] != '/';
      char *child = static_cast<char *>(
          malloc(length + static_cast<size_t>(slash) + child_length + 1));
      if (!child) {
        errno = ENOMEM;
        result = -1;
      } else {
        memcpy(child, path, length);
        if (slash)
          child[length++] = '/';
        memcpy(child + length, name->text, child_length + 1);
        if (!(ctx.flags & FTW_MOUNT)) {
          result = walk(ctx, child, level + 1, &current);
        } else {
          struct stat child_stat;
          int stat_error = (ctx.flags & FTW_PHYS) ? lstat(child, &child_stat)
                                                  : stat(child, &child_stat);
          if (stat_error != 0 || child_stat.st_dev == ctx.root_device)
            result = walk(ctx, child, level + 1, &current);
        }
        free(child);
      }
    }
    free(name);
  }
  if (result != 0)
    return result;
  return (ctx.flags & FTW_DEPTH) ? callback(ctx, path, &st, FTW_DP, level) : 0;
}
} // namespace ftw_internal
} // namespace LIBC_NAMESPACE_DECL
#endif
