#ifndef LLVM_LIBC_SRC_STDLIB_MKDTEMP_H
#define LLVM_LIBC_SRC_STDLIB_MKDTEMP_H

#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
char *mkdtemp(char *);
}

#endif
