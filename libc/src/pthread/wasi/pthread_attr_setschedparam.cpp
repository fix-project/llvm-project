#include "src/pthread/pthread_attr_setschedparam.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_setschedparam,
                   (pthread_attr_t *__restrict attr,
                    const struct sched_param *__restrict schedparam)) {
  (void)attr;
  (void)schedparam;
  libc_errno = ENOTSUP;
  return ENOTSUP;
}

} // namespace LIBC_NAMESPACE_DECL
