#include "src/sys/socket/accept4.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, accept4, (int sockfd, struct sockaddr *__restrict addr, socklen_t *__restrict addrlen, int flags)) {
  // WASI does not provide this interface.
  (void)sockfd;
  (void)addr;
  (void)addrlen;
  (void)flags;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
