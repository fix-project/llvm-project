//===-- WASIp1 execvpe implementation ------------------------------------===//

#include "src/unistd/execvpe.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, execvpe,
                   (const char *file, char *const argv[], char *const envp[])) {
  (void)file;
  (void)argv;
  (void)envp;
  libc_errno = ENOSYS;
  return -1;
}
} // namespace LIBC_NAMESPACE_DECL
