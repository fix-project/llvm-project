#include "src/stdlib/srandom.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(void, srandom, (unsigned int seed)) { srand(seed); }
} // namespace LIBC_NAMESPACE_DECL
