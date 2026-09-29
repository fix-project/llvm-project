#include "src/sys/socket/getsockopt.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getsockopt, (int sockfd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen)) {
  // WASI does not provide this interface.
  (void)sockfd;
  (void)level;
  (void)optname;
  (void)optval;
  (void)optlen;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
