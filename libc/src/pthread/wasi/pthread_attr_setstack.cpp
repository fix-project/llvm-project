#include "src/pthread/pthread_attr_setstack.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_setstack,
                   (pthread_attr_t * attr, void *stack, size_t stacksize)) {
  if (attr == nullptr || stack == nullptr || stacksize < PTHREAD_STACK_MIN) {
    return EINVAL;
  }
  attr->__stack = stack;
  attr->__stacksize = stacksize;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
