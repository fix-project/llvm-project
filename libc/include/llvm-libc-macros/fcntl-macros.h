#ifndef LLVM_LIBC_MACROS_FCNTL_MACROS_H
#define LLVM_LIBC_MACROS_FCNTL_MACROS_H

#if defined(__linux__)
#include "linux/fcntl-macros.h"
#elif defined(__wasi__)
#include "wasi/fcntl-macros.h"
#endif

#endif // LLVM_LIBC_MACROS_FCNTL_MACROS_H
