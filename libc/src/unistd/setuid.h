#ifndef LLVM_LIBC_SRC_UNISTD_SETUID_H
#define LLVM_LIBC_SRC_UNISTD_SETUID_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
int setuid(uid_t uid);
}
#endif
