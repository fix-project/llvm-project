#include "src/signal/sigprocmask.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

static int do_sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
  if (how != SIG_BLOCK && how != SIG_UNBLOCK && how != SIG_SETMASK) {
    libc_errno = EINVAL;
    return -1;
  }
  sigset_t &current = wasi::signal_state().mask;
  if (oldset != nullptr)
    *oldset = current;
  if (set == nullptr)
    return 0;
  switch (how) {
  case SIG_BLOCK:
    for (size_t i = 0; i < __NSIGSET_WORDS; ++i)
      current.__signals[i] |= set->__signals[i];
    break;
  case SIG_UNBLOCK:
    for (size_t i = 0; i < __NSIGSET_WORDS; ++i)
      current.__signals[i] &= ~set->__signals[i];
    break;
  case SIG_SETMASK:
    current = *set;
    break;
  }
  return 0;
}

LLVM_LIBC_FUNCTION(int, sigprocmask,
                   (int how, const sigset_t *set, sigset_t *oldset)) {
  return do_sigprocmask(how, set, oldset);
}

} // namespace LIBC_NAMESPACE_DECL
