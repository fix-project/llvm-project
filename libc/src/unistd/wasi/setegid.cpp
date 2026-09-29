#include "src/unistd/setegid.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, setegid, (gid_t gid)) {
  (void)gid;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
