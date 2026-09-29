#include "src/time/tzset.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(void, tzset, ()) {}
} // namespace LIBC_NAMESPACE_DECL
