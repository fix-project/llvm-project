#include "src/sys/mman/munlockall.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, munlockall, (void)) {
  // WASI does not provide this interface.
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
