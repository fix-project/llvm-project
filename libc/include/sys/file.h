#ifndef LLVM_LIBC_INCLUDE_SYS_FILE_H
#define LLVM_LIBC_INCLUDE_SYS_FILE_H

#include "__llvm-libc-common.h"

#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8

__BEGIN_C_DECLS
int flock(int, int) __NOEXCEPT;
__END_C_DECLS

#endif
