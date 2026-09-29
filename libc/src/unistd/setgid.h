#ifndef LLVM_LIBC_SRC_UNISTD_SETGID_H
#define LLVM_LIBC_SRC_UNISTD_SETGID_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
int setgid(gid_t gid);
}
#endif
