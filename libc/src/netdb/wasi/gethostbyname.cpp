//===-- WASIp1 gethostbyname implementation
//------------------------------------===//

#include "src/netdb/gethostbyname.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
extern "C" {
int h_errno = 0;
}

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(struct hostent *, gethostbyname, (const char *name)) {
  (void)name;
  h_errno = NO_RECOVERY;
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
