#include "src/ftw/ftw.h"
#include "src/__support/common.h"
#include "src/ftw/walk.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, ftw,
                   (const char *path,
                    int (*fn)(const char *, const struct stat *, int),
                    int nopenfd)) {
  if (!path || !fn || nopenfd < 1) {
    errno = EINVAL;
    return -1;
  }
  ftw_internal::Context ctx = {fn, nullptr, 0, 0};
  return ftw_internal::walk(ctx, path, 0);
}
} // namespace LIBC_NAMESPACE_DECL
