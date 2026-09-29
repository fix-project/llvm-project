if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
endif()

add_entrypoint_object(
  gai_strerror
  SRCS
    gai_strerror.cpp
  HDRS
    gai_strerror.h
  DEPENDS
    libc.include.netdb
    libc.src.__support.common
)

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

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    gethostbyname
    ALIAS
    DEPENDS
      .wasi.gethostbyname
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    gethostbyaddr
    ALIAS
    DEPENDS
      .wasi.gethostbyaddr
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    getservent
    ALIAS
    DEPENDS
      .wasi.getservent
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    getservbyname
    ALIAS
    DEPENDS
      .wasi.getservbyname
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    getservbyport
    ALIAS
    DEPENDS
      .wasi.getservbyport
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    getprotobyname
    ALIAS
    DEPENDS
      .wasi.getprotobyname
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    setservent
    ALIAS
    DEPENDS
      .wasi.setservent
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    endservent
    ALIAS
    DEPENDS
      .wasi.endservent
  )
endif()
