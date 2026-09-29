#include "src/time/strptime.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(char *, strptime,
                   (const char *input, const char *format, struct tm *result)) {
  (void)input;
  (void)format;
  (void)result;
  libc_errno = ENOSYS;
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
