#include "src/pthread/pthread_attr_setstacksize.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_setstacksize,
                   (pthread_attr_t * attr, size_t stacksize)) {
  if (attr == nullptr || stacksize < PTHREAD_STACK_MIN) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  attr->__stacksize = stacksize;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
