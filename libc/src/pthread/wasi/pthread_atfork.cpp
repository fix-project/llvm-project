#include "src/pthread/pthread_atfork.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_atfork,
                   (__atfork_callback_t prepare, __atfork_callback_t parent,
                    __atfork_callback_t child)) {
  // WASI has no fork; callbacks are accepted but will never run.
  (void)prepare;
  (void)parent;
  (void)child;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
