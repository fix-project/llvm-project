//===-- WASIp1 getservbyname implementation
//------------------------------------===//

#include "src/netdb/getservbyname.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(struct servent *, getservbyname,
                   (const char *name, const char *proto)) {
  (void)name;
  (void)proto;
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
