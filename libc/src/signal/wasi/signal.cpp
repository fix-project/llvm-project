#include "src/signal/signal.h"

#include "hdr/signal_macros.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/signal/sigaction.h"

namespace LIBC_NAMESPACE_DECL {

using signal_handler = void (*)(int);

LLVM_LIBC_FUNCTION(signal_handler, signal,
                   (int signum, signal_handler handler)) {
  struct sigaction action, old;
  action.sa_handler = handler;
  action.sa_flags = SA_RESTART;
  return LIBC_NAMESPACE::sigaction(signum, &action, &old) == -1
             ? SIG_ERR
             : old.sa_handler;
}

} // namespace LIBC_NAMESPACE_DECL
