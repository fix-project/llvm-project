#include "src/pthread/pthread_rwlockattr_setkind_np.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlockattr_setkind_np,
                   (pthread_rwlockattr_t * attr, int pref)) {
  if (attr == nullptr || (pref != PTHREAD_RWLOCK_PREFER_READER_NP &&
                          pref != PTHREAD_RWLOCK_PREFER_WRITER_NP &&
                          pref != PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP)) {
    return EINVAL;
  }
  attr->pref = pref;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
