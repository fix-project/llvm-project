#include "src/pthread/pthread_setspecific.h"

#include "pthread_wasi_state.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_setspecific,
                   (pthread_key_t key, const void *value)) {
  if (key >= wasi::PTHREAD_MAX_KEYS) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  wasi::tss_values[key] = value;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
