#include "src/unistd/sync.h"
#include "src/__support/common.h"

namespace LIBC_NAMESPACE_DECL {
// WASIp1 has no process-wide dirty-buffer cache to flush. Individual
// descriptors support fsync and fdatasync.
LLVM_LIBC_FUNCTION(void, sync, ()) {}
} // namespace LIBC_NAMESPACE_DECL
