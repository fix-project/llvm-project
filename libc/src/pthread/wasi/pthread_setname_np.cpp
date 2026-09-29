#include "src/pthread/pthread_setname_np.h"

#include "pthread_wasi_state.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_setname_np, (pthread_t, const char *name)) {
  if (name == nullptr) {
    return EINVAL;
  }
  unsigned i = 0;
  for (; i + 1 < wasi::PTHREAD_NAME_MAX && name[i] != '\0'; ++i)
    wasi::thread_name[i] = name[i];
  wasi::thread_name[i] = '\0';
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
