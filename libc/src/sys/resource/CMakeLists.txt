if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
endif()

add_entrypoint_object(
  getrlimit
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getrlimit
)

if (LIBC_TARGET_OS STREQUAL "wasi")
  add_entrypoint_object(
    getrusage
    ALIAS
    DEPENDS
      .wasi.getrusage
  )
endif()

add_entrypoint_object(
  setrlimit
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.setrlimit
)
