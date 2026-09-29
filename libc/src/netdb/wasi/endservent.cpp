//===-- WASIp1 endservent implementation
//------------------------------------===//

#include "src/netdb/endservent.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(void, endservent, ()) { libc_errno = ENOSYS; }
} // namespace LIBC_NAMESPACE_DECL
