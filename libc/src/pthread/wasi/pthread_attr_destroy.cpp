#include "src/pthread/pthread_attr_destroy.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_attr_destroy, (pthread_attr_t * attr)) {
  (void)attr;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
