#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {
// The public header only declares mlock2 when SYS_mlock2 is available.
int mlock2(const void *addr, size_t len, int flags);
} // namespace LIBC_NAMESPACE_DECL

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, mlock2, (const void *addr, size_t len, int flags)) {
  // WASI does not provide this interface.
  (void)addr;
  (void)len;
  (void)flags;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
