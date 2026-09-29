# WASI (`wasm32-wasip1`) port

The WASI port implements the standard C library on top of the WASI preview 1
system interface using raw WebAssembly imports (no wasi-libc dependency).
The ABI layer lives in `libc/src/__support/OSUtil/wasi/`.

## Platform characteristics

- **Single-threaded.** WASI preview 1 has no threads. The build uses
  `LIBC_THREAD_MODE_SINGLE`; `pthread_*` entrypoints provide single-threaded
  semantics (`pthread_create` fails with `EAGAIN`).
- **Single process.** There is no process model. `getpid`/`getppid` return
  `1`; `getuid`/`geteuid`/`getgid`/`getegid` return `0`.
- **Synchronous signals only.** There is no asynchronous signal delivery.
  Signal set/mask/handler bookkeeping is kept consistent, and `raise()` or
  `kill()` targeting the calling process delivers the signal synchronously:
  installed handlers run, `SIG_IGN`/ignore dispositions are honored, and
  terminate-by-default signals exit via `proc_raise` (falling back to
  `proc_exit` with the signal number). `kill()` to any other pid fails with
  `ESRCH`.
- **Memory mapping is emulated.** Anonymous mappings use the libc heap.
  File mappings copy data on creation; writable shared mappings write it
  back on `msync` or `munmap`. Memory protection cannot be enforced.
- **No sockets.** The WASI preview 1 `sock_*` calls are not exposed by
  mainstream runtimes for core modules (wasmtime provides sockets only
  through the component model), so the socket entrypoints remain stubs.
- **`epoll` is emulated.** Epoll instances live in userspace;
  `epoll_wait()`/`epoll_pwait()`/`epoll_pwait2()` block in `poll()` (built
  on `poll_oneoff`). `EPOLLONESHOT` can be rearmed with `MOD`; `EPOLLET` is accepted but
  behaves level-triggered.
- **`poll()` follows wasi-libc semantics.** An entry with a valid descriptor
  but no read/write event requested returns `ENOSYS` (exceptional conditions
  cannot be detected without a subscription), and a call with nothing to wait
  for and no timeout returns `ENOTSUP`. Invalid descriptors report `POLLNVAL`
  per entry (wasmtime's `poll_oneoff` rejects the whole call on a bad fd, so
  descriptors are pre-validated).
- **`posix_fadvise`/`posix_fallocate` map onto `fd_advise`/`fd_allocate`**,
  matching wasi-libc (error numbers are returned directly, not via `errno`).
- **`fstatvfs`/`statvfs` are best-effort.** WASI preview 1 exposes no
  filesystem-capacity query (wasi-libc does not provide `statvfs` at all), so
  block sizes and free-space figures are approximations.
- **`sendfile` is emulated** with a `pread`/`write` loop (8 KiB chunks),
  preserving the input offset semantics and returning a short count at EOF.
- **`dup`/`dup2`/`dup3` are emulated** by re-opening the tracked path of an
  open descriptor; they work for descriptors opened through this libc.
  They do not share the original open file description (and therefore do not
  share offsets), and fail if the path has been renamed or removed. Descriptor
  allocation enforces the POSIX lowest-free-fd rule on top of the runtime's
  allocator.

## Stub semantics table

| Entrypoint(s)                              | Return value        | `errno` |
| ------------------------------------------ | ------------------- | ------- |
| `fork`, `vfork`                             | `-1`                | `ENOSYS` |
| `execv`, `execve`, `execl`, `execlp`, `execvp`, `execvpe` | `-1` | `ENOSYS` |
| `kill` | self-targeting: delivered synchronously; other pids `-1` | `ESRCH` (other pids), `EINVAL` (invalid sig) |
| `raise` | delivered synchronously to the calling thread | `EINVAL` (invalid sig) |
| `sigaction`, `signal` (registering handlers) | bookkeeping; handlers fire on synchronous `raise`/`kill` delivery | — |
| `sigprocmask`, `pthread_sigmask`            | `0` (mask tracked in userspace) | — |
| `mmap`, `munmap`, `msync` | emulated with heap memory and file I/O; shared writes are flushed on `msync`/`munmap` | mapping and I/O errors |
| `mprotect` | `0` (protection changes are not enforced) | — |
| `mremap`, `madvise`, `mlock`, `munlock`, `mlock2`, `mlockall`, `munlockall`, `shm_open`, `shm_unlink` | `-1` | `ENOSYS` |
| `posix_madvise`                              | `ENOSYS` (returns error code directly) | — |
| `fcntl` `F_DUPFD`/`F_DUPFD_CLOEXEC`          | `-1`                | `EINVAL` |
| `fcntl` `F_GETFD`/`F_SETFD`                  | `FD_CLOEXEC`/`0` (close-on-exec is a no-op; fd still validated) | `EBADF` (invalid fd) |
| `pthread_create`                             | `EAGAIN`            | `EAGAIN` |
| `getpid`, `getppid`                          | `1`                 | — |
| `getuid`, `geteuid`, `getgid`, `getegid`     | `0`                 | — |
| `wait`, `waitpid`, `wait3`, `wait4`          | `-1`                | `ECHILD` |
| `setjmp`, `longjmp`, `sigsetjmp`, `siglongjmp` | provided via Wasm EH when built with `LIBC_WASM_ENABLE_SJLJ=ON`; consumers need `-mllvm -wasm-enable-sjlj -mllvm -wasm-use-legacy-eh=false -mexception-handling -mmultivalue -mreference-types` | — |
| `termios` family (`tcgetattr`, `tcsetattr`, `cfgetispeed`, ...) | `-1` (`0` for `cfgetispeed`/`cfgetospeed`) | `ENOSYS` |
| `dlopen`, `dlsym`, `dlclose`, `dlerror`      | `NULL`/nonzero/`"unsupported"` (no dynamic loading on WASI) | — |
| `posix_spawn`                                | `ENOSYS` (returns error code directly); file-action setup functions are available | — |
| `getitimer`, `setitimer`                     | `-1`                | `ENOSYS` |
| `gettid`                                     | `1` (single implicit thread) | — |
| `socket`, `connect`, `bind`, `listen`, `accept`, `send`, `recv`, etc. | `-1` | `ENOSYS` |
| `epoll_create`, `epoll_create1`, `epoll_ctl`, `epoll_wait`, `epoll_pwait`, `epoll_pwait2` | emulated over `poll()`; up to 8 instances, 128 watched fds each | standard errnos (`EBADF`, `EINVAL`, `EEXIST`, `ENOENT`) |

## Testing

The WASI tests can run under an emulator such as wasmtime. Tests requiring
subprocess support (death tests) or real signal/process behavior are gated off
on WASI. WASI-specific behavior is covered by `libc/test/src/wasi/`. The
current full-build CI job builds the WASI libc but does not run `check-libc`;
running the suite also requires a WASI C++ runtime and compiler builtins.

To reproduce a full test run locally:

1. Build `compiler-rt` builtins and a `libc++`/`libc++abi` pair for
   `wasm32-wasip1` against this libc.
2. Configure the runtimes build with
   `-DLIBC_TARGET_TRIPLE=wasm32-wasip1 -DLLVM_LIBC_FULL_BUILD=ON
   -DLIBC_TEST_SKIP_DEATH_TESTS=ON` and
   `-DCMAKE_CROSSCOMPILING_EMULATOR=<path-to-wasmtime-wrapper>`.
3. `ninja check-libc`.
