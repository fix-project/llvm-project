#include "src/pthread/pthread_exit.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/stdlib/exit.h"
#include <pthread.h>

static __llvm_libc_cleanup_frame *cleanup_top;

extern "C" void
__llvm_libc_pthread_cleanup_push(__llvm_libc_cleanup_frame *frame,
                                 void (*routine)(void *), void *argument) {
  frame->routine = routine;
  frame->argument = argument;
  frame->previous = cleanup_top;
  cleanup_top = frame;
}

extern "C" void
__llvm_libc_pthread_cleanup_pop(__llvm_libc_cleanup_frame *frame, int execute) {
  if (cleanup_top != frame)
    return;
  cleanup_top = frame->previous;
  if (execute)
    frame->routine(frame->argument);
}

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void, pthread_exit, (void *retval)) {
  // WASI is single-threaded: terminating the calling thread terminates the
  // process. The exit status cannot be conveyed through a join retval.
  (void)retval;
  while (cleanup_top) {
    __llvm_libc_cleanup_frame *frame = cleanup_top;
    __llvm_libc_pthread_cleanup_pop(frame, 1);
  }
  LIBC_NAMESPACE::exit(0);
  __builtin_unreachable();
}

} // namespace LIBC_NAMESPACE_DECL
