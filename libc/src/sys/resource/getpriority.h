#ifndef LLVM_LIBC_SRC_SYS_RESOURCE_GETPRIORITY_H
#define LLVM_LIBC_SRC_SYS_RESOURCE_GETPRIORITY_H
#include "src/__support/macros/config.h"
namespace LIBC_NAMESPACE_DECL {
int getpriority(int, int);
}
#endif
