#include "src/unistd/fdatasync.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(int, fdatasync, (int fd)) {
  wasi::__wasi_errno_t error =
      wasi::__wasi_fd_datasync(static_cast<wasi::__wasi_fd_t>(fd));
  if (error != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(error);
    return -1;
  }
  return 0;
}
} // namespace LIBC_NAMESPACE_DECL
