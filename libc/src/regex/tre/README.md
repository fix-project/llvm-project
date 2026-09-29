This directory contains the TRE POSIX regular-expression engine imported from
musl libc's `src/regex` directory. The import was made from musl commit
`c4e1bb3994c14ed5112c894d15a451bf00f0d501`. The original BSD-style
copyright and license notices remain
in the source files; `LICENSE.TRE` is installed alongside LLVM libc for binary
redistribution.

LLVM libc uses its own public `regex_t` layout and flag values. The only local
functional changes to the imported engine are in `tre.h`: its private state
field is `regex_t.__internal`, the musl-specific `hidden` annotations were
removed, and its `NDEBUG` definition is guarded to avoid conflicting with
build flags that already define it.
