#include "src/sys/socket/setsockopt.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, setsockopt, (int sockfd, int level, int optname, const void *optval, socklen_t optlen)) {
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
