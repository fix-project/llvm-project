#include "src/stdio/tmpnam.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/stdio/generic/temp_path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(char *, tmpnam, (char *dest)) {
  static char buffer[L_tmpnam];
  static unsigned int sequence;
  if (sequence >= TMP_MAX) {
    libc_errno = EEXIST;
    return nullptr;
  }
  char path[L_tmpnam];
  if (!make_temp_template(path, sizeof(path)))
    return nullptr;
  size_t length = strlen(path);
  if (length + 4 >= sizeof(path)) {
    libc_errno = ENAMETOOLONG;
    return nullptr;
  }
  memmove(path + length - 6 + 4, path + length - 6, 7);
  unsigned int serial = sequence;
  for (int i = 3; i >= 0; --i) {
    path[length - 6 + i] = static_cast<char>('0' + (serial % 10));
    serial /= 10;
  }
  int fd = mkstemp(path);
  if (fd < 0)
    return nullptr;
  int close_result = close(fd);
  int close_error = libc_errno;
  int unlink_result = unlink(path);
  if (close_result != 0) {
    libc_errno = close_error;
    return nullptr;
  }
  if (unlink_result != 0)
    return nullptr;
  char *result = dest ? dest : buffer;
  memcpy(result, path, strlen(path) + 1);
  ++sequence;
  return result;
}
} // namespace LIBC_NAMESPACE_DECL
