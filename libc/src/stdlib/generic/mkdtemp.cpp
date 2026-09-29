#include "src/stdlib/mkdtemp.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/sys/stat/mkdir.h"
#include "src/unistd/getentropy.h"
#include <errno.h>
#include <stdint.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(char *, mkdtemp, (char *path)) {
  if (!path) {
    libc_errno = EINVAL;
    return nullptr;
  }
  size_t length = 0;
  while (path[length])
    ++length;
  size_t first_x = length;
  while (first_x && path[first_x - 1] == 'X')
    --first_x;
  if (length - first_x < 6) {
    libc_errno = EINVAL;
    return nullptr;
  }

  constexpr char alphabet[] =
      "-._0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  for (unsigned int attempt = 0; attempt < 256; ++attempt) {
    for (size_t i = first_x; i < length; ++i) {
      uint8_t byte;
      if (LIBC_NAMESPACE::getentropy(&byte, 1) != 0)
        return nullptr;
      path[i] = alphabet[byte % (sizeof(alphabet) - 1)];
    }
    if (LIBC_NAMESPACE::mkdir(path, 0700) == 0)
      return path;
    if (libc_errno != EEXIST)
      return nullptr;
  }
  libc_errno = EEXIST;
  return nullptr;
}

} // namespace LIBC_NAMESPACE_DECL
