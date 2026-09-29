//===-- WASIp1 getservbyport implementation
//------------------------------------===//

#include "src/netdb/getservbyport.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(struct servent *, getservbyport,
                   (int port, const char *proto)) {
  (void)port;
  (void)proto;
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
