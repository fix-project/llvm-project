#ifndef LLVM_LIBC_INCLUDE_ICONV_H
#define LLVM_LIBC_INCLUDE_ICONV_H

#include "__llvm-libc-common.h"
#include "llvm-libc-types/size_t.h"

typedef void *iconv_t;

__BEGIN_C_DECLS
iconv_t iconv_open(const char *, const char *) __NOEXCEPT;
size_t iconv(iconv_t, char **, size_t *, char **, size_t *) __NOEXCEPT;
int iconv_close(iconv_t) __NOEXCEPT;
__END_C_DECLS

#endif
