#include "src/pthread/pthread_cond_signal.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_cond_signal, (pthread_cond_t * cond)) {
  (void)cond;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
