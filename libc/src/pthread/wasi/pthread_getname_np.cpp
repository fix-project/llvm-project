#include "src/pthread/pthread_getname_np.h"

#include "pthread_wasi_state.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_getname_np,
                   (pthread_t, char *name, size_t len)) {
  if (name == nullptr || len == 0) {
    return EINVAL;
  }
  size_t i = 0;
  for (; i + 1 < len && wasi::thread_name[i] != '\0'; ++i)
    name[i] = wasi::thread_name[i];
  name[i] = '\0';
  if (wasi::thread_name[i] != '\0') {
    return ERANGE;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
