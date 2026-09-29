//===-- WASIp1 pthread_mutex_consistent implementation
//------------------------------------===//

#include "src/pthread/pthread_mutex_consistent.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, pthread_mutex_consistent, (pthread_mutex_t * mutex)) {
  (void)mutex;
  return EINVAL;
}
} // namespace LIBC_NAMESPACE_DECL
