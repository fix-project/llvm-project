//===-- WASIp1 gethostbyaddr implementation
//------------------------------------===//

#include "src/netdb/gethostbyaddr.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(struct hostent *, gethostbyaddr,
                   (const void *addr, socklen_t len, int type)) {
  (void)addr;
  (void)len;
  (void)type;
  h_errno = NO_RECOVERY;
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
