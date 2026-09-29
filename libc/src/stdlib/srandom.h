#ifndef LLVM_LIBC_SRC_STDLIB_SRANDOM_H
#define LLVM_LIBC_SRC_STDLIB_SRANDOM_H
#include "src/__support/macros/config.h"
#include <stdlib.h>
namespace LIBC_NAMESPACE_DECL {
void srandom(unsigned int seed);
}
#endif
