if(NOT TARGET libc.src.__support.OSUtil.osutil)
  message(STATUS "libc.src.__support.OSUtil.osutil is not avaiable. "
                 "Skip unistd targets.")
  return()
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
endif()

add_entrypoint_object(
  _exit
  SRCS
    _exit.cpp
  HDRS
    _exit.h
  DEPENDS
    libc.include.unistd
    libc.src.__support.OSUtil.osutil
)

add_entrypoint_object(
  alarm
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.alarm
)

add_entrypoint_object(
  access
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.access
)

add_entrypoint_object(
  chdir
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.chdir
)

add_entrypoint_object(
  chown
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.chown
)

add_entrypoint_object(
  lchown
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.lchown
)

add_entrypoint_object(
  sleep
  SRCS
    sleep.cpp
  HDRS
    sleep.h
  DEPENDS
    libc.hdr.types.struct_timespec
    libc.src.__support.common
    libc.src.time.nanosleep
)

add_entrypoint_object(
  close
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.close
)

add_entrypoint_object(
  dup
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.dup
)

add_entrypoint_object(
  dup2
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.dup2
)

add_entrypoint_object(
  dup3
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.dup3
)

add_entrypoint_object(
  faccessat
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.faccessat
)

add_entrypoint_object(
  fchdir
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.fchdir
)

add_entrypoint_object(
  fchown
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.fchown
)

add_entrypoint_object(
  fchownat
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.fchownat
)

add_entrypoint_object(
  chroot
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.chroot
)

add_entrypoint_object(
  getgroups
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getgroups
)

add_entrypoint_object(
  fork
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.fork
)

add_entrypoint_object(
  fpathconf
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.fpathconf
)

add_entrypoint_object(
  execv
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.execv
)

add_entrypoint_object(
  execlp
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.execlp
)

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    execl
    ALIAS
    DEPENDS
      .wasi.execl
  )
  add_entrypoint_object(
    getpgrp
    ALIAS
    DEPENDS
      .wasi.getpgrp
  )
  add_entrypoint_object(
    sethostname
    ALIAS
    DEPENDS
      .wasi.sethostname
  )
endif()

add_entrypoint_object(
  execvp
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.execvp
)

add_entrypoint_object(
  execve
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.execve
)

add_entrypoint_object(
  fsync
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.fsync
)

if(LIBC_TARGET_OS_IS_WASI)
  foreach(entrypoint fdatasync sync nice)
    add_entrypoint_object(
      ${entrypoint}
      ALIAS
      DEPENDS
        .wasi.${entrypoint}
    )
  endforeach()
endif()

add_entrypoint_object(
  ftruncate
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.ftruncate
)

add_entrypoint_object(
  getcwd
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getcwd
)

add_entrypoint_object(
  gethostname
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.gethostname
)

if(LIBC_TARGET_OS_IS_WASI)
  foreach(entrypoint confstr getlogin getlogin_r)
    add_entrypoint_object(
      ${entrypoint}
      ALIAS
      DEPENDS
        .wasi.${entrypoint}
    )
  endforeach()
endif()

add_entrypoint_object(
  getpagesize
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getpagesize
)

add_entrypoint_object(
  getpid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getpid
)

add_entrypoint_object(
  getppid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getppid
)

add_entrypoint_object(
  getsid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getsid
)

add_entrypoint_object(
  geteuid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.geteuid
)

add_entrypoint_object(
  getuid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getuid
)

add_entrypoint_object(
  getgid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getgid
)

add_entrypoint_object(
  isatty
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.isatty
)

add_entrypoint_object(
  link
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.link
)

add_entrypoint_object(
  linkat
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.linkat
)

add_entrypoint_object(
  lseek
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.lseek
)

add_entrypoint_object(
  pathconf
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.pathconf
)


add_entrypoint_object(
  pipe
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.pipe
)

add_entrypoint_object(
  pipe2
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.pipe2
)

add_entrypoint_object(
  pread
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.pread
)

add_entrypoint_object(
  pwrite
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.pwrite
)

add_entrypoint_object(
  read
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.read
)

add_entrypoint_object(
  readlink
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.readlink
)

add_entrypoint_object(
  readlinkat
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.readlinkat
)

add_entrypoint_object(
  rmdir
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.rmdir
)

add_entrypoint_object(
  setsid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.setsid
)

add_entrypoint_object(
  symlink
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.symlink
)

add_entrypoint_object(
  symlinkat
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.symlinkat
)

add_entrypoint_object(
  __llvm_libc_syscall
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.__llvm_libc_syscall
)

add_entrypoint_object(
  sysconf
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.sysconf
)

add_entrypoint_object(
  truncate
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.truncate
)

add_entrypoint_object(
  unlink
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.unlink
)

add_entrypoint_object(
  unlinkat
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.unlinkat
)

add_entrypoint_object(
  write
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.write
)

add_entrypoint_object(
  environ
  SRCS
    environ.cpp
  HDRS
    environ.h
  DEPENDS
    libc.src.__support.common
    libc.src.__support.macros.config
)

add_entrypoint_object(
  getopt
  SRCS
    getopt.cpp
  HDRS
    getopt.h
  DEPENDS
    libc.include.unistd
    libc.hdr.unistd_macros
    libc.hdr.types.FILE
    libc.src.__support.common
    libc.src.__support.macros.config
    libc.src.__support.CPP.optional
    libc.src.__support.CPP.string_view
    libc.src.__support.File.file
    libc.src.stdio.fprintf
    libc.src.stdio.stderr
)

add_entrypoint_object(
  swab
  SRCS
    swab.cpp
  HDRS
    swab.h
  DEPENDS
    libc.include.unistd
    libc.src.__support.CPP.optional
)

# These aren't actual external per-se, but this is just an easy way to create
# targets that don't do anything. They exist to be referenced in entrypoints.txt
# so that the header will properly expose their definitions. Their declaration
# is in getopt.cpp.
add_entrypoint_external(
  optarg
)

add_entrypoint_external(
  optind
)

add_entrypoint_external(
  optopt
)

add_entrypoint_external(
  opterr
)

add_entrypoint_object(
  gettid
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.gettid
)

add_entrypoint_object(
  getentropy
  ALIAS
  DEPENDS
    .${LIBC_TARGET_OS}.getentropy
)

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    getegid
    ALIAS
    DEPENDS
      .wasi.getegid
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    execvpe
    ALIAS
    DEPENDS
      .wasi.execvpe
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    ttyname
    ALIAS
    DEPENDS
      .wasi.ttyname
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    ttyname_r
    ALIAS
    DEPENDS
      .wasi.ttyname_r
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    vfork
    ALIAS
    DEPENDS
      .wasi.vfork
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    setuid
    ALIAS
    DEPENDS
      .wasi.setuid
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    setgid
    ALIAS
    DEPENDS
      .wasi.setgid
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    setegid
    ALIAS
    DEPENDS
      .wasi.setegid
  )
endif()

if(LIBC_TARGET_OS_IS_WASI)
  add_entrypoint_object(
    seteuid
    ALIAS
    DEPENDS
      .wasi.seteuid
  )
endif()
