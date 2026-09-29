#include "src/sys/mman/memfd_create.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, memfd_create, (const char *name, unsigned int flags)) {
  // WASI does not provide this interface.
  (void)name;
  (void)flags;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
