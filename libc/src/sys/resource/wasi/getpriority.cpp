#include "src/sys/resource/getpriority.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, getpriority, (int which, int who)) {
  (void)which;
  (void)who;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
