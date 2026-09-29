#include "src/unistd/setgid.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, setgid, (gid_t gid)) {
  (void)gid;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
