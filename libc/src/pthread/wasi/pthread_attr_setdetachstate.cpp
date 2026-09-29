#include "src/pthread/pthread_attr_setdetachstate.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_setdetachstate,
                   (pthread_attr_t * attr, int detach_state)) {
  if (attr == nullptr || (detach_state != PTHREAD_CREATE_JOINABLE &&
                          detach_state != PTHREAD_CREATE_DETACHED)) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  attr->__detachstate = detach_state;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
