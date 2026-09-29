//===-- WASIp1 getservent implementation
//------------------------------------===//

#include "src/netdb/getservent.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(struct servent *, getservent, ()) {
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
