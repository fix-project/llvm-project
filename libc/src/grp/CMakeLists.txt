if(LIBC_TARGET_OS STREQUAL "wasi")
  add_entrypoint_object(
    getgrgid
    SRCS
      wasi/group.c
    DEPENDS
    libc.include.stdint
      libc.include.errno
      libc.include.grp
      libc.include.string
  )

  add_entrypoint_object(
    getgrnam
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    getgrgid_r
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    getgrnam_r
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    endgrent
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    setgrent
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    getgrent
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    initgroups
    ALIAS
    DEPENDS
      .getgrgid
  )

  add_entrypoint_object(
    getgrouplist
    ALIAS
    DEPENDS
      .getgrgid
  )
endif()
