# Platform Support

Development is currently mostly focused on Linux. MacOS and Windows has
partial support, but has bitrot and isn't being tested continuously.

LLVM-libc is currently being integrated into Android and Fuchsia operating
systems via {doc}`overlay mode <overlay_mode>`.

For Linux, we support kernel versions as listed on
[kernel.org](https://kernel.org/), including `longterm` (not past EOL
date), `stable`, and `mainline` versions. We actively adopt new features
from `linux-next`.

For Windows, we plan to support products within their lifecycle. Please refer to
[Search Product and Services Lifecycle Information](https://learn.microsoft.com/en-us/lifecycle/products/?products=windows) for more information.

LLVM-libc does not guarantee backward compatibility with operating systems that
have reached their EOL. Compatibility patches for obsolete operating systems
will not be accepted.

For GPU, reference {doc}`our GPU docs <gpu/index>`.

For WASI (`wasm32-wasip1`), the standard C library is implemented on top of
the WASI preview 1 system interface. The port targets the WebAssembly System
Interface as implemented by runtimes such as wasmtime. Notable characteristics
of the WASI port:

- WASI preview 1 has no notion of processes or threads, so the port is
  single-threaded (`LIBC_THREAD_MODE_SINGLE`) and process-related entrypoints
  (`fork`, `exec*`, ...) return `-1` with `errno` set to `ENOSYS` where the
  underlying platform cannot provide the functionality. `wait`/`waitpid`/
  `wait4` return `-1` with `ECHILD` since no child processes can exist.
- There is no asynchronous signal delivery. Signal-related entrypoints keep
  consistent bookkeeping (signal sets, masks, stored handlers); `raise` and
  self-targeting `kill` deliver the signal synchronously by invoking the
  installed handler.
- `setjmp`/`longjmp` (and `sigsetjmp`/`siglongjmp`) are supported via
  WebAssembly exception handling when the libc is built with
  `LIBC_WASM_ENABLE_SJLJ=ON`. Consumers must compile with
  `-mllvm -wasm-enable-sjlj -mllvm -wasm-use-legacy-eh=false` (plus
  `-mexception-handling -mmultivalue -mreference-types`).
- Memory mapping (`mmap`, `mprotect`, ...) and shared memory are not provided
  by WASI and fail with `ENOSYS`.
- `pthread` entrypoints follow single-threaded semantics: `pthread_create`
  fails with `EAGAIN` and all other entrypoints operate on the single
  implicit thread.

See `libc/config/wasi/README.md` for the full stub semantics table.
