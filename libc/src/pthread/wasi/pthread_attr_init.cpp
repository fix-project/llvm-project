#include "src/pthread/pthread_attr_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_init, (pthread_attr_t * attr)) {
  if (attr == nullptr) {
    return EINVAL;
  }
  attr->__detachstate = PTHREAD_CREATE_JOINABLE;
  attr->__stack = nullptr;
  attr->__stacksize = 2 * 1024 * 1024;
  attr->__guardsize = 4096;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
