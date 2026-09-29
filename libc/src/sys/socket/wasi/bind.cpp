#include "src/sys/socket/bind.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, bind, (int socket, const struct sockaddr *address, socklen_t address_len)) {
  // WASI does not provide this interface.
  (void)socket;
  (void)address;
  (void)address_len;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
