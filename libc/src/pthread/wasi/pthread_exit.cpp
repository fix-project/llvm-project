#include "src/pthread/pthread_exit.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/stdlib/exit.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void, pthread_exit, (void *retval)) {
  // WASI is single-threaded: terminating the calling thread terminates the
  // process. The exit status cannot be conveyed through a join retval.
  (void)retval;
  LIBC_NAMESPACE::exit(0);
  __builtin_unreachable();
}

} // namespace LIBC_NAMESPACE_DECL
