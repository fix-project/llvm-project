#include "src/sys/time/settimeofday.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, settimeofday,
                   (const struct timeval *time, const void *timezone)) {
  (void)time;
  (void)timezone;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
