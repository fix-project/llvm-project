//===-- WASIp1 getegid implementation ------------------------------------===//

#include "src/unistd/getegid.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(gid_t, getegid, ()) { return 0; }
} // namespace LIBC_NAMESPACE_DECL
