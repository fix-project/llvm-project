#ifndef LLVM_LIBC_INCLUDE_MNTENT_H
#define LLVM_LIBC_INCLUDE_MNTENT_H

#include "__llvm-libc-common.h"
#include "llvm-libc-types/FILE.h"

struct mntent {
  char *mnt_fsname;
  char *mnt_dir;
  char *mnt_type;
  char *mnt_opts;
  int mnt_freq;
  int mnt_passno;
};

__BEGIN_C_DECLS
FILE *setmntent(const char *, const char *) __NOEXCEPT;
struct mntent *getmntent(FILE *) __NOEXCEPT;
struct mntent *getmntent_r(FILE *, struct mntent *, char *, int) __NOEXCEPT;
int endmntent(FILE *) __NOEXCEPT;
int addmntent(FILE *, const struct mntent *) __NOEXCEPT;
char *hasmntopt(const struct mntent *, const char *) __NOEXCEPT;
__END_C_DECLS

#endif
