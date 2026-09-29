#include "src/sys/socket/socketpair.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, socketpair, (int domain, int type, int protocol, int sv[2])) {
  // WASI does not provide this interface.
  (void)domain;
  (void)type;
  (void)protocol;
  (void)sv;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
