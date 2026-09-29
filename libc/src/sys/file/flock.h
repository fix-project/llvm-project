#ifndef LLVM_LIBC_SRC_SYS_FILE_FLOCK_H
#define LLVM_LIBC_SRC_SYS_FILE_FLOCK_H
#include "src/__support/macros/config.h"
namespace LIBC_NAMESPACE_DECL {
int flock(int, int);
}
#endif
