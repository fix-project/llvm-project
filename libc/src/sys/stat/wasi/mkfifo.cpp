//===-- WASIp1 mkfifo implementation ------------------------------------===//

#include "src/sys/stat/mkfifo.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, mkfifo, (const char *path, mode_t mode)) {
  (void)path;
  (void)mode;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
