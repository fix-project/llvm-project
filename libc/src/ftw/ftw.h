#ifndef LLVM_LIBC_SRC_FTW_FTW_H
#define LLVM_LIBC_SRC_FTW_FTW_H
#include "src/__support/macros/config.h"
#include <ftw.h>
namespace LIBC_NAMESPACE_DECL {
int ftw(const char *, int (*)(const char *, const struct stat *, int), int);
}
#endif
