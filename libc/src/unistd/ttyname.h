//===-- ttyname entrypoint -----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_TTYNAME_H
#define LLVM_LIBC_SRC_UNISTD_TTYNAME_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
char *ttyname(int fd);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_UNISTD_TTYNAME_H
