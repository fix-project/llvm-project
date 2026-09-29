#include "src/sys/mman/remap_file_pages.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, remap_file_pages, (void *addr, size_t size, int prot, size_t pgoff, int flags)) {
  // WASI does not provide this interface.
  (void)addr;
  (void)size;
  (void)prot;
  (void)pgoff;
  (void)flags;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
