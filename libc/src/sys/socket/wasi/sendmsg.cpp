#include "src/sys/socket/sendmsg.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, sendmsg, (int sockfd, const struct msghdr *msg, int flags)) {
  // WASI does not provide this interface.
  (void)sockfd;
  (void)msg;
  (void)flags;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
