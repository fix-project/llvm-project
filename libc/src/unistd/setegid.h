#ifndef LLVM_LIBC_SRC_UNISTD_SETEGID_H
#define LLVM_LIBC_SRC_UNISTD_SETEGID_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
int setegid(gid_t gid);
}
#endif
