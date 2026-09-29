//===-- execvpe entrypoint -----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_EXECVPE_H
#define LLVM_LIBC_SRC_UNISTD_EXECVPE_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
int execvpe(const char *file, char *const argv[], char *const envp[]);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_UNISTD_EXECVPE_H
