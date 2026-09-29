#include "src/signal/sigaltstack.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, sigaltstack,
                   (const stack_t *ss, stack_t *old_ss)) {
  wasi::SignalState &state = wasi::signal_state();
  if (old_ss != nullptr) {
    if (!state.altstack_set) {
      // Report a disabled alternate stack when none has been installed.
      old_ss->ss_sp = nullptr;
      old_ss->ss_flags = SS_DISABLE;
      old_ss->ss_size = 0;
    } else {
      *old_ss = state.altstack;
    }
  }
  if (ss == nullptr)
    return 0;
  if (ss->ss_flags == SS_DISABLE) {
    state.altstack = *ss;
    state.altstack_set = true;
    return 0;
  }
  if (ss->ss_flags != 0 || ss->ss_sp == nullptr || ss->ss_size < MINSIGSTKSZ) {
    libc_errno = ENOMEM;
    return -1;
  }
  state.altstack = *ss;
  state.altstack_set = true;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
