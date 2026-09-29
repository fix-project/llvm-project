#include "src/sys/epoll/epoll_pwait2.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/sys/epoll/wasi/epoll_state.h"

#include <limits.h>
#include <signal.h>
#include <sys/epoll.h>
#include <time.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, epoll_pwait2,
                   (int epfd, epoll_event *events, int maxevents,
                    const timespec *timeout, const sigset_t *sigmask)) {
  (void)sigmask;
  int timeout_ms;
  if (timeout == nullptr) {
    timeout_ms = -1;
  } else if (timeout->tv_sec < 0 || timeout->tv_nsec < 0 ||
             timeout->tv_nsec >= 1000000000) {
    libc_errno = EINVAL;
    return -1;
  } else if (timeout->tv_sec >= INT_MAX / 1000) {
    timeout_ms = INT_MAX;
  } else {
    timeout_ms = static_cast<int>(timeout->tv_sec) * 1000 +
                 static_cast<int>((timeout->tv_nsec + 999999) / 1000000);
  }
  return wasi::epoll_wait_impl(epfd, events, maxevents, timeout_ms);
}

} // namespace LIBC_NAMESPACE_DECL
