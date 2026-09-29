#include "src/pthread/pthread_self.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(pthread_t, pthread_self, ()) {
  // The single implicit thread has a stable, non-null identity.
  static int main_thread_token;
  return &main_thread_token;
}

} // namespace LIBC_NAMESPACE_DECL
