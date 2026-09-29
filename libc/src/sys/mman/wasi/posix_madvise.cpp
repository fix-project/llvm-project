#include "src/sys/mman/posix_madvise.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, posix_madvise, (void *addr, size_t size, int advice)) {
  // The mmap emulation has no page tables to advise about; accept the
  // request as a no-op.
  (void)addr;
  (void)size;
  (void)advice;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
