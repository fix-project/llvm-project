//===-- WASIp1 ttyname implementation ------------------------------------===//

#include "src/unistd/ttyname.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/unistd/isatty.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(char *, ttyname, (int fd)) {
  if (LIBC_NAMESPACE::isatty(fd))
    libc_errno = ENOTSUP;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
