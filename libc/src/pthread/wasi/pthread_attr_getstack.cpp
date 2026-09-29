#include "src/pthread/pthread_attr_getstack.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_getstack,
                   (const pthread_attr_t *__restrict attr,
                    void **__restrict stackaddr,
                    size_t *__restrict stacksize)) {
  if (attr == nullptr || stackaddr == nullptr || stacksize == nullptr) {
    return EINVAL;
  }
  *stackaddr = attr->__stack;
  *stacksize = attr->__stacksize;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
