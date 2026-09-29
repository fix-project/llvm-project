#ifndef LLVM_LIBC_SRC_SYS_TIME_SETTIMEOFDAY_H
#define LLVM_LIBC_SRC_SYS_TIME_SETTIMEOFDAY_H
#include "src/__support/macros/config.h"
#include <sys/time.h>
namespace LIBC_NAMESPACE_DECL {
int settimeofday(const struct timeval *time, const void *timezone);
}
#endif
