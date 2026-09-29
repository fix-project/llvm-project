#ifndef LLVM_LIBC_SRC_UNISTD_VFORK_H
#define LLVM_LIBC_SRC_UNISTD_VFORK_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
pid_t vfork();
}
#endif
