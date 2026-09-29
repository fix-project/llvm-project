#include "src/pthread/pthread_mutex_timedlock.h"
#include "src/__support/common.h"
#include <errno.h>
#include <time.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, pthread_mutex_timedlock,
                   (pthread_mutex_t * mutex, const struct timespec *abstime)) {
  if (!mutex || !abstime || abstime->tv_nsec < 0 ||
      abstime->tv_nsec >= 1000000000L)
    return EINVAL;
  if (!mutex->__locked) {
    mutex->__locked = 1;
    mutex->__lock_count = 1;
    return 0;
  }
  if (mutex->__recursive) {
    ++mutex->__lock_count;
    return 0;
  }
  if (mutex->__error_checking)
    return EDEADLK;
  struct timespec now;
  if (clock_gettime(CLOCK_REALTIME, &now) != 0)
    return EINVAL;
  while (now.tv_sec < abstime->tv_sec ||
         (now.tv_sec == abstime->tv_sec && now.tv_nsec < abstime->tv_nsec)) {
    struct timespec remaining = {abstime->tv_sec - now.tv_sec,
                                 abstime->tv_nsec - now.tv_nsec};
    if (remaining.tv_nsec < 0) {
      --remaining.tv_sec;
      remaining.tv_nsec += 1000000000L;
    }
    nanosleep(&remaining, nullptr);
    if (clock_gettime(CLOCK_REALTIME, &now) != 0)
      return EINVAL;
  }
  return ETIMEDOUT;
}
} // namespace LIBC_NAMESPACE_DECL
