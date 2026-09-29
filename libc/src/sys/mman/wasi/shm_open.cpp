#include "src/sys/mman/shm_open.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, shm_open, (const char *name, int oflag, mode_t mode)) {
  // WASI does not provide this interface.
  (void)name;
  (void)oflag;
  (void)mode;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
