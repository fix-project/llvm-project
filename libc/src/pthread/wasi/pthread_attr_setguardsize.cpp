#include "src/pthread/pthread_attr_setguardsize.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_setguardsize,
                   (pthread_attr_t * attr, size_t guardsize)) {
  if (attr == nullptr) {
    return EINVAL;
  }
  attr->__guardsize = guardsize;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
