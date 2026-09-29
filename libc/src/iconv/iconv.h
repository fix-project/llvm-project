#ifndef LLVM_LIBC_SRC_ICONV_ICONV_H
#define LLVM_LIBC_SRC_ICONV_ICONV_H

#include "src/__support/macros/config.h"
#include <iconv.h>

namespace LIBC_NAMESPACE_DECL {
iconv_t iconv_open(const char *, const char *);
size_t iconv(iconv_t, char **, size_t *, char **, size_t *);
int iconv_close(iconv_t);
} // namespace LIBC_NAMESPACE_DECL

#endif
