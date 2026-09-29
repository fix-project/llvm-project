//===-- WASIp1 getprotobyname implementation
//------------------------------------===//

#include "src/netdb/getprotobyname.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(struct protoent *, getprotobyname, (const char *name)) {
  (void)name;
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
