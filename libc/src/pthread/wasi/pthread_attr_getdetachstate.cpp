#include "src/pthread/pthread_attr_getdetachstate.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_getdetachstate,
                   (const pthread_attr_t *attr, int *detach_state)) {
  if (attr == nullptr || detach_state == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *detach_state = attr->__detachstate;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
