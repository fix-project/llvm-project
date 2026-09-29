#include "src/ftw/nftw.h"
#include "src/__support/common.h"
#include "src/ftw/walk.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, nftw,
                   (const char *path,
                    int (*fn)(const char *, const struct stat *, int,
                              struct FTW *),
                    int nopenfd, int flags)) {
  if (!path || !fn || nopenfd < 1 ||
      (flags & ~(FTW_PHYS | FTW_MOUNT | FTW_CHDIR | FTW_DEPTH))) {
    errno = EINVAL;
    return -1;
  }
  ftw_internal::Context ctx = {nullptr, fn, flags, 0};
  return ftw_internal::walk(ctx, path, 0);
}
} // namespace LIBC_NAMESPACE_DECL
