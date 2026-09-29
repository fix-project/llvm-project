//===-- WASIp1 mknodat implementation ------------------------------------===//

#include "src/sys/stat/mknodat.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, mknodat,
                   (int dirfd, const char *path, mode_t mode, dev_t dev)) {
  (void)dirfd;
  (void)path;
  (void)mode;
  (void)dev;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
