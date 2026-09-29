if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
endif()

add_entrypoint_object(
  getaddrinfo
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getaddrinfo
)

add_entrypoint_object(
  freeaddrinfo
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.freeaddrinfo
)

add_entrypoint_object(
  getnameinfo
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getnameinfo
)
