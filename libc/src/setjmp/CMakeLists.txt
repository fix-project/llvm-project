# Process architecture-specific subdirectory FIRST to avoid missing targets.

# Then process OS-specific subdirectory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  if(TARGET libc.src.setjmp.${LIBC_TARGET_OS}.sigsetjmp_epilogue)
    add_object_library(
      sigsetjmp_epilogue
      ALIAS
      DEPENDS
        .${LIBC_TARGET_OS}.sigsetjmp_epilogue
    )
  endif()
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_ARCHITECTURE})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_ARCHITECTURE})
endif()

add_entrypoint_object(
  setjmp
  ALIAS
  DEPENDS
    .${LIBC_TARGET_ARCHITECTURE}.setjmp
)

add_entrypoint_object(
  longjmp
  ALIAS
  DEPENDS
    .${LIBC_TARGET_ARCHITECTURE}.longjmp
)

if (LIBC_TARGET_OS_IS_WASI)
  # On WebAssembly, sigsetjmp/siglongjmp are mapped onto setjmp/longjmp by
  # macros in <setjmp.h> so that the wasm-enable-sjlj pass can lower the
  # calls in the user's frame.  The wasm32 entrypoints are trap stubs that
  # only exist so the symbols resolve; they are built only when the sjlj
  # lowering is enabled, in which case the aliases below resolve to them.
  add_entrypoint_object(
    sigsetjmp
    ALIAS
    DEPENDS
      .${LIBC_TARGET_ARCHITECTURE}.sigsetjmp
  )

  add_entrypoint_object(
    siglongjmp
    ALIAS
    DEPENDS
      .${LIBC_TARGET_ARCHITECTURE}.siglongjmp
  )
elseif (TARGET libc.src.setjmp.sigsetjmp_epilogue)
  add_entrypoint_object(
    siglongjmp
    SRCS
      siglongjmp.cpp
    HDRS
      siglongjmp.h
    DEPENDS
      .longjmp
  )

  add_entrypoint_object(
    sigsetjmp
    ALIAS
    DEPENDS
      .${LIBC_TARGET_ARCHITECTURE}.sigsetjmp
  )
endif()
