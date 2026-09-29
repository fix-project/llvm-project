#ifndef LLVM_LIBC_SRC_TIME_TZSET_H
#define LLVM_LIBC_SRC_TIME_TZSET_H
#include "src/__support/macros/config.h"
#include <time.h>
namespace LIBC_NAMESPACE_DECL {
void tzset();
}
#endif
