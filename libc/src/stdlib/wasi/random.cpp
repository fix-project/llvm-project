#include "src/stdlib/random.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(long, random, ()) { return static_cast<long>(rand()); }
} // namespace LIBC_NAMESPACE_DECL
