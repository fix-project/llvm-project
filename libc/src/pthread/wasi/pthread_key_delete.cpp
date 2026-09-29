#include "src/pthread/pthread_key_delete.h"

#include "pthread_wasi_state.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_key_delete, (pthread_key_t key)) {
  if (key >= wasi::PTHREAD_MAX_KEYS) {
    return EINVAL;
  }
  wasi::tss_dtors[key] = nullptr;
  wasi::tss_values[key] = nullptr;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
