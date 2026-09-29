#ifndef LLVM_LIBC_SRC_SIGNAL_SIGSUSPEND_H
#define LLVM_LIBC_SRC_SIGNAL_SIGSUSPEND_H
#include "src/__support/macros/config.h"
#include <signal.h>
namespace LIBC_NAMESPACE_DECL {
int sigsuspend(const sigset_t *mask);
}
#endif
