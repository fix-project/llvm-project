#include "src/signal/kill.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/unistd/getpid.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, kill, (pid_t pid, int sig)) {
  if (sig < 0 || sig > NSIG - 1) {
    libc_errno = EINVAL;
    return -1;
  }
  // WASI preview1 has no process table.  Signalling the calling process
  // (pid == getpid(), the process group pid == 0, or the all-processes
  // target pid == -1) is delivered synchronously; any other target cannot
  // exist and reports ESRCH.
  if (pid == 0 || pid == -1 || pid == getpid()) {
    if (sig != 0)
      wasi::deliver_self(sig);
    return 0;
  }
  libc_errno = ESRCH;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
