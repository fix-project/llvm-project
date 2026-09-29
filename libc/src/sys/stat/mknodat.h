//===-- mknodat entrypoint -----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_STAT_MKNODAT_H
#define LLVM_LIBC_SRC_SYS_STAT_MKNODAT_H
#include "src/__support/macros/config.h"
#include <sys/stat.h>
namespace LIBC_NAMESPACE_DECL {
int mknodat(int dirfd, const char *path, mode_t mode, dev_t dev);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_SYS_STAT_MKNODAT_H
