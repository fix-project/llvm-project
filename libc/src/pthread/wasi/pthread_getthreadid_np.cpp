#include "src/pthread/pthread_getthreadid_np.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(pthread_id_np_t, pthread_getthreadid_np, ()) {
  return 1; // The single implicit thread.
}

} // namespace LIBC_NAMESPACE_DECL
