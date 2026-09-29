//===-- mkfifo entrypoint -----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_STAT_MKFIFO_H
#define LLVM_LIBC_SRC_SYS_STAT_MKFIFO_H
#include "src/__support/macros/config.h"
#include <sys/stat.h>
namespace LIBC_NAMESPACE_DECL {
int mkfifo(const char *path, mode_t mode);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_SYS_STAT_MKFIFO_H
