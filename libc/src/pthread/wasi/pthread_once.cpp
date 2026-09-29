#include "src/pthread/pthread_once.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_once, (pthread_once_t * flag, __pthread_once_func_t func)) {
  if (flag == nullptr || func == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  if (flag->__word == 0) {
    flag->__word = 1;
    func();
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
