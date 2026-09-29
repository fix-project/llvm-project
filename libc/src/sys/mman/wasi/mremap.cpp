#include "src/sys/mman/mremap.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void *, mremap, (void *old_address, size_t old_size, size_t new_size, int flags, ...)) {
  // WASI does not provide this interface.
  (void)old_address;
  (void)old_size;
  (void)new_size;
  (void)flags;
  libc_errno = ENOSYS;
  return MAP_FAILED;
}

} // namespace LIBC_NAMESPACE_DECL
