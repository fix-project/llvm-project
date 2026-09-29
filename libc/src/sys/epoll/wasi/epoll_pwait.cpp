#include "src/sys/epoll/epoll_pwait.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/sys/epoll/wasi/epoll_state.h"

#include <signal.h>
#include <sys/epoll.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, epoll_pwait,
                   (int epfd, struct epoll_event *events, int maxevents,
                    int timeout, const sigset_t *sigmask)) {
  // Synchronous signal delivery means there is no window to atomically
  // unblock signals around the wait; the mask is accepted and ignored.
  (void)sigmask;
  return wasi::epoll_wait_impl(epfd, events, maxevents, timeout);
}

} // namespace LIBC_NAMESPACE_DECL
