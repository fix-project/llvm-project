#ifndef LLVM_LIBC_SRC_UNISTD_SETEUID_H
#define LLVM_LIBC_SRC_UNISTD_SETEUID_H
#include "src/__support/macros/config.h"
#include <unistd.h>
namespace LIBC_NAMESPACE_DECL {
int seteuid(uid_t uid);
}
#endif
