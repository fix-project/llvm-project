if(LIBC_TARGET_OS STREQUAL "wasi")
  add_entrypoint_object(
    getpwuid
    SRCS
      wasi/passwd.c
    DEPENDS
      libc.include.pwd
      libc.include.string
  )

  add_entrypoint_object(
    getpwnam
    ALIAS
    DEPENDS
      .getpwuid
  )

  add_entrypoint_object(
    getpwuid_r
    ALIAS
    DEPENDS
      .getpwuid
  )

  add_entrypoint_object(
    getpwnam_r
    ALIAS
    DEPENDS
      .getpwuid
  )

  foreach(entrypoint getpwent setpwent endpwent)
    add_entrypoint_object(
      ${entrypoint}
      ALIAS
      DEPENDS
        .getpwuid
    )
  endforeach()
endif()
