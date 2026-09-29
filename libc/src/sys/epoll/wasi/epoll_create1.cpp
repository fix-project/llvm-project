#include "src/sys/epoll/epoll_create1.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/sys/epoll/wasi/epoll_state.h"

#include <sys/epoll.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, epoll_create1, (int flags)) {
  if (flags & ~EPOLL_CLOEXEC) {
    libc_errno = EINVAL;
    return -1;
  }
  return wasi::epoll_create_instance();
}

} // namespace LIBC_NAMESPACE_DECL
