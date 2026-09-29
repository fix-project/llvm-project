//===-- WASIp1 setservent implementation
//------------------------------------===//

#include "src/netdb/setservent.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(void, setservent, (int stayopen)) {
  (void)stayopen;
  libc_errno = ENOSYS;
}
} // namespace LIBC_NAMESPACE_DECL
