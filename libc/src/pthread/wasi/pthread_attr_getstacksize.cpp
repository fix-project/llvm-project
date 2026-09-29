#include "src/pthread/pthread_attr_getstacksize.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_getstacksize,
                   (const pthread_attr_t *__restrict attr,
                    size_t *__restrict stacksize)) {
  if (attr == nullptr || stacksize == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *stacksize = attr->__stacksize;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
