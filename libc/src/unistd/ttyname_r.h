//===-- ttyname_r entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_TTYNAME_R_H
#define LLVM_LIBC_SRC_UNISTD_TTYNAME_R_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
int ttyname_r(int fd, char *buf, size_t buflen);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_UNISTD_TTYNAME_R_H
