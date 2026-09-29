#include "src/pthread/pthread_attr_getguardsize.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_getguardsize,
                   (const pthread_attr_t *__restrict attr,
                    size_t *__restrict guardsize)) {
  if (attr == nullptr || guardsize == nullptr) {
    return EINVAL;
  }
  *guardsize = attr->__guardsize;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
