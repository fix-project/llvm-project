#include "src/sys/socket/recvfrom.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, recvfrom, (int sockfd, void *__restrict buf, size_t len, int flags, sockaddr *__restrict src_addr, socklen_t *__restrict addrlen)) {
  // WASI does not provide this interface.
  (void)sockfd;
  (void)buf;
  (void)len;
  (void)flags;
  (void)src_addr;
  (void)addrlen;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
