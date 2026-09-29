#ifndef LLVM_LIBC_SRC_TIME_STRPTIME_H
#define LLVM_LIBC_SRC_TIME_STRPTIME_H
#include "src/__support/macros/config.h"
#include <time.h>
namespace LIBC_NAMESPACE_DECL {
char *strptime(const char *input, const char *format, struct tm *result);
}
#endif
