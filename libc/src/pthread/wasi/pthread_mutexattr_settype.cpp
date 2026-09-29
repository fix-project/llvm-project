#include "src/pthread/pthread_mutexattr_settype.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_settype,
                   (pthread_mutexattr_t * attr, int type)) {
  if (attr == nullptr || type < PTHREAD_MUTEX_NORMAL ||
      type > PTHREAD_MUTEX_RECURSIVE) {
    return EINVAL;
  }
  *attr = (*attr & ~0x3u) | static_cast<unsigned>(type);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
