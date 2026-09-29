#include "src/sys/mman/pkey_mprotect.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pkey_mprotect, (void *addr, size_t len, int prot, int pkey)) {
  // WASI does not provide this interface.
  (void)addr;
  (void)len;
  (void)prot;
  (void)pkey;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
