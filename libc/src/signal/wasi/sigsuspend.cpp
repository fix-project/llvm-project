#include "src/signal/sigsuspend.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, sigsuspend, (const sigset_t *mask)) {
  (void)mask;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
