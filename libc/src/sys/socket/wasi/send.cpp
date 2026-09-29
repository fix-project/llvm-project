#include "src/sys/socket/send.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, send, (int sockfd, const void *buf, size_t len, int flags)) {
  // WASI does not provide this interface.
  (void)sockfd;
  (void)buf;
  (void)len;
  (void)flags;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
