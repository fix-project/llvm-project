#include "src/sys/epoll/epoll_wait.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/sys/epoll/wasi/epoll_state.h"

#include <sys/epoll.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, epoll_wait,
                   (int epfd, epoll_event *events, int maxevents,
                    int timeout)) {
  return wasi::epoll_wait_impl(epfd, events, maxevents, timeout);
}

} // namespace LIBC_NAMESPACE_DECL
