#include "src/pthread/pthread_self.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(pthread_t, pthread_self, ()) {
  // The single implicit thread is the "main" thread; its handle is zero.
  pthread_t self = {};
  return self;
}

} // namespace LIBC_NAMESPACE_DECL
