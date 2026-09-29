#ifndef LLVM_LIBC_FTW_H
#define LLVM_LIBC_FTW_H

#include "__llvm-libc-common.h"
#include <sys/stat.h>

#define FTW_F 0
#define FTW_D 1
#define FTW_DNR 2
#define FTW_NS 3
#define FTW_SL 4
#define FTW_DP 5
#define FTW_SLN 6

#define FTW_PHYS 1
#define FTW_MOUNT 2
#define FTW_CHDIR 4
#define FTW_DEPTH 8

struct FTW {
  int base;
  int level;
};

__BEGIN_C_DECLS
int ftw(const char *, int (*)(const char *, const struct stat *, int),
        int) __NOEXCEPT;
int nftw(const char *,
         int (*)(const char *, const struct stat *, int, struct FTW *), int,
         int) __NOEXCEPT;
__END_C_DECLS

#endif
