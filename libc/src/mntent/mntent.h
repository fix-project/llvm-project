#ifndef LLVM_LIBC_SRC_MNTENT_MNTENT_H
#define LLVM_LIBC_SRC_MNTENT_MNTENT_H

#include "src/__support/macros/config.h"
#include <mntent.h>

namespace LIBC_NAMESPACE_DECL {
FILE *setmntent(const char *, const char *);
struct mntent *getmntent(FILE *);
struct mntent *getmntent_r(FILE *, struct mntent *, char *, int);
int endmntent(FILE *);
int addmntent(FILE *, const struct mntent *);
char *hasmntopt(const struct mntent *, const char *);
} // namespace LIBC_NAMESPACE_DECL

#endif
