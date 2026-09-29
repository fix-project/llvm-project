#include "src/pthread/pthread_getspecific.h"

#include "pthread_wasi_state.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void *, pthread_getspecific, (pthread_key_t key)) {
  if (key >= wasi::PTHREAD_MAX_KEYS)
    return nullptr;
  return const_cast<void *>(wasi::tss_values[key]);
}

} // namespace LIBC_NAMESPACE_DECL
