# setjmp/longjmp for WebAssembly require the exception-handling based sjlj
# lowering; only build them when the feature is enabled. When disabled, the
# top-level alias to .wasm32.setjmp/.wasm32.longjmp resolves to nothing, so
# the entrypoints are simply excluded from the build.
if(LIBC_WASM_ENABLE_SJLJ)
  add_entrypoint_object(
    setjmp
    SRCS
      setjmp.cpp
    HDRS
      ../setjmp_impl.h
      wasm_sjlj.h
    DEPENDS
    libc.include.stdint
      libc.hdr.types.jmp_buf
      libc.src.__support.common
  )

  add_entrypoint_object(
    longjmp
    SRCS
      longjmp.cpp
    HDRS
      ../longjmp.h
      wasm_sjlj.h
    DEPENDS
    libc.include.stdint
      libc.hdr.types.jmp_buf
      libc.src.__support.common
  )

  # The real work happens via the sigsetjmp/siglongjmp macros in <setjmp.h>,
  # which redirect to setjmp/longjmp so the wasm-enable-sjlj pass can lower
  # the call in the user's frame.  The entrypoints below are trap stubs for
  # code that references the symbols without the header; the pass must not
  # see any libc definitions, so no per-file pass flags are needed here.
  add_entrypoint_object(
    sigsetjmp
    SRCS
      sigsetjmp.cpp
    HDRS
      ../sigsetjmp.h
    DEPENDS
      libc.hdr.types.jmp_buf
      libc.hdr.types.sigjmp_buf
      libc.src.__support.common
  )

  add_entrypoint_object(
    siglongjmp
    SRCS
      siglongjmp.cpp
    HDRS
      ../siglongjmp.h
    DEPENDS
      libc.hdr.types.jmp_buf
      libc.src.__support.common
  )
endif()
