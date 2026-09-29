#include "src/pthread/pthread_getunique_np.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_getunique_np,
                   (const pthread_t *__restrict thread,
                    pthread_id_np_t *__restrict id)) {
  if (thread == nullptr || id == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *id = 1; // The single implicit thread.
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
