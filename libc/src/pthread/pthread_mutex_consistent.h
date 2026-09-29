//===-- pthread_mutex_consistent entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_PTHREAD_PTHREAD_MUTEX_CONSISTENT_H
#define LLVM_LIBC_SRC_PTHREAD_PTHREAD_MUTEX_CONSISTENT_H
#include "src/__support/macros/config.h"
#include <pthread.h>
namespace LIBC_NAMESPACE_DECL {
int pthread_mutex_consistent(pthread_mutex_t *mutex);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_PTHREAD_PTHREAD_MUTEX_CONSISTENT_H
