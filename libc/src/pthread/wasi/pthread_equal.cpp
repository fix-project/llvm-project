#include "src/pthread/pthread_equal.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_equal, (pthread_t lhs, pthread_t rhs)) {
  return lhs == rhs ? 1 : 0;
}

} // namespace LIBC_NAMESPACE_DECL
