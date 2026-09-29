#ifndef LLVM_LIBC_SRC_UNISTD_FDATASYNC_H
#define LLVM_LIBC_SRC_UNISTD_FDATASYNC_H
#include "src/__support/macros/config.h"
namespace LIBC_NAMESPACE_DECL {
int fdatasync(int);
}
#endif
