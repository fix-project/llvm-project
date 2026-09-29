#ifndef LLVM_LIBC_MACROS_ERROR_NUMBER_MACROS_H
#define LLVM_LIBC_MACROS_ERROR_NUMBER_MACROS_H

#ifdef __linux__
#include "linux/error-number-macros.h"
#elif defined(__wasi__)
#include "wasi/error-number-macros.h"
#endif

#endif // LLVM_LIBC_MACROS_ERROR_NUMBER_MACROS_H
