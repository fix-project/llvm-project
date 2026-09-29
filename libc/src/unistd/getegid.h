//===-- getegid entrypoint -----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_GETEGID_H
#define LLVM_LIBC_SRC_UNISTD_GETEGID_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
gid_t getegid();
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_UNISTD_GETEGID_H
