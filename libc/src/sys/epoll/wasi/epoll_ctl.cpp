#include "src/sys/epoll/epoll_ctl.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/sys/epoll/wasi/epoll_state.h"

#include <sys/epoll.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, epoll_ctl,
                   (int epfd, int op, int fd, struct epoll_event *event)) {
  return wasi::epoll_ctl_impl(epfd, op, fd, event);
}

} // namespace LIBC_NAMESPACE_DECL
