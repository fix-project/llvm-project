#include "src/sys/socket/socket.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, socket, (int domain, int type, int protocol)) {
  // WASI does not provide this interface.
  (void)domain;
  (void)type;
  (void)protocol;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
