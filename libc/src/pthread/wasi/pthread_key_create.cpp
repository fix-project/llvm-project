#include "src/pthread/pthread_key_create.h"

#include "pthread_wasi_state.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_key_create,
                   (pthread_key_t * key, __pthread_tss_dtor_t dtor)) {
  if (key == nullptr) {
    return EINVAL;
  }
  for (unsigned i = 0; i < wasi::PTHREAD_MAX_KEYS; ++i) {
    if (wasi::tss_dtors[i] == nullptr && wasi::tss_values[i] == nullptr) {
      wasi::tss_dtors[i] = dtor;
      *key = i;
      return 0;
    }
  }
  return EAGAIN;
}

} // namespace LIBC_NAMESPACE_DECL
