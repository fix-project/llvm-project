//===-- WASIp1 ttyname_r implementation
//------------------------------------===//

#include "src/unistd/ttyname_r.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/unistd/isatty.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, ttyname_r, (int fd, char *buf, size_t buflen)) {
  (void)buf;
  (void)buflen;
  int previous = libc_errno;
  if (LIBC_NAMESPACE::isatty(fd)) {
    libc_errno = previous;
    return ENOTSUP;
  }
  int result = libc_errno;
  libc_errno = previous;
  return result;
}
} // namespace LIBC_NAMESPACE_DECL
