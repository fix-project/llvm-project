#ifndef LLVM_LIBC_SRC_STDIO_GENERIC_TEMP_PATH_H
#define LLVM_LIBC_SRC_STDIO_GENERIC_TEMP_PATH_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace LIBC_NAMESPACE_DECL {
inline bool make_temp_template(char *path, size_t capacity) {
  const char *directory = getenv("TMPDIR");
  if (!directory || !directory[0])
    directory = P_tmpdir;
  size_t length = strlen(directory);
  bool slash = length && directory[length - 1] != '/';
  constexpr char suffix[] = "tmp.XXXXXX";
  if (length + static_cast<size_t>(slash) + sizeof(suffix) > capacity) {
    errno = ENAMETOOLONG;
    return false;
  }
  memcpy(path, directory, length);
  if (slash)
    path[length++] = '/';
  memcpy(path + length, suffix, sizeof(suffix));
  return true;
}
} // namespace LIBC_NAMESPACE_DECL

#endif
