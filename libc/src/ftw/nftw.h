#ifndef LLVM_LIBC_SRC_FTW_NFTW_H
#define LLVM_LIBC_SRC_FTW_NFTW_H
#include "src/__support/macros/config.h"
#include <ftw.h>
namespace LIBC_NAMESPACE_DECL {
int nftw(const char *,
         int (*)(const char *, const struct stat *, int, struct FTW *), int,
         int);
}
#endif
