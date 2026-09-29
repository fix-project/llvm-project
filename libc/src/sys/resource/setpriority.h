#ifndef LLVM_LIBC_SRC_SYS_RESOURCE_SETPRIORITY_H
#define LLVM_LIBC_SRC_SYS_RESOURCE_SETPRIORITY_H
#include "src/__support/macros/config.h"
namespace LIBC_NAMESPACE_DECL {
int setpriority(int, int, int);
}
#endif
