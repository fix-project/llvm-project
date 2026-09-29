#include "src/sys/mman/pkey_set.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pkey_set, (int pkey, unsigned int access_rights)) {
  // WASI does not provide this interface.
  (void)pkey;
  (void)access_rights;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
