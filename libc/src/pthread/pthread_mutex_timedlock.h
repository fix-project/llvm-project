#ifndef LLVM_LIBC_SRC_PTHREAD_PTHREAD_MUTEX_TIMEDLOCK_H
#define LLVM_LIBC_SRC_PTHREAD_PTHREAD_MUTEX_TIMEDLOCK_H
#include "src/__support/macros/config.h"
#include <pthread.h>
namespace LIBC_NAMESPACE_DECL {
int pthread_mutex_timedlock(pthread_mutex_t *, const struct timespec *);
}
#endif
