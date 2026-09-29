if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LIBC_TARGET_OS})

  add_entrypoint_object(
    pthread_atfork
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_atfork
  )

  add_entrypoint_object(
    pthread_attr_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_destroy
  )

  add_entrypoint_object(
    pthread_attr_getdetachstate
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_getdetachstate
  )

  add_entrypoint_object(
    pthread_attr_getguardsize
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_getguardsize
  )

  add_entrypoint_object(
    pthread_attr_getschedparam
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_getschedparam
  )

  add_entrypoint_object(
    pthread_attr_getstack
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_getstack
  )

  add_entrypoint_object(
    pthread_attr_getstacksize
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_getstacksize
  )

  add_entrypoint_object(
    pthread_attr_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_init
  )

  add_entrypoint_object(
    pthread_attr_setdetachstate
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_setdetachstate
  )

  add_entrypoint_object(
    pthread_attr_setguardsize
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_setguardsize
  )

  add_entrypoint_object(
    pthread_attr_setschedparam
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_setschedparam
  )

  add_entrypoint_object(
    pthread_attr_setstack
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_setstack
  )

  add_entrypoint_object(
    pthread_attr_setstacksize
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_attr_setstacksize
  )

  add_entrypoint_object(
    pthread_barrier_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_barrier_destroy
  )

  add_entrypoint_object(
    pthread_barrier_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_barrier_init
  )

  add_entrypoint_object(
    pthread_barrier_wait
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_barrier_wait
  )

  add_entrypoint_object(
    pthread_cond_broadcast
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_broadcast
  )

  add_entrypoint_object(
    pthread_cond_clockwait
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_clockwait
  )

  add_entrypoint_object(
    pthread_cond_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_destroy
  )

  add_entrypoint_object(
    pthread_cond_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_init
  )

  add_entrypoint_object(
    pthread_cond_signal
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_signal
  )

  add_entrypoint_object(
    pthread_cond_timedwait
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_timedwait
  )

  add_entrypoint_object(
    pthread_cond_wait
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_cond_wait
  )

  add_entrypoint_object(
    pthread_condattr_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_condattr_destroy
  )

  add_entrypoint_object(
    pthread_condattr_getclock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_condattr_getclock
  )

  add_entrypoint_object(
    pthread_condattr_getpshared
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_condattr_getpshared
  )

  add_entrypoint_object(
    pthread_condattr_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_condattr_init
  )

  add_entrypoint_object(
    pthread_condattr_setclock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_condattr_setclock
  )

  add_entrypoint_object(
    pthread_condattr_setpshared
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_condattr_setpshared
  )

  add_entrypoint_object(
    pthread_create
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_create
  )

  add_entrypoint_object(
    pthread_detach
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_detach
  )

  add_entrypoint_object(
    pthread_equal
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_equal
  )

  add_entrypoint_object(
    pthread_exit
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_exit
  )

  add_entrypoint_object(
    pthread_getname_np
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_getname_np
  )

  add_entrypoint_object(
    pthread_getspecific
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_getspecific
  )

  add_entrypoint_object(
    pthread_getthreadid_np
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_getthreadid_np
  )

  add_entrypoint_object(
    pthread_getunique_np
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_getunique_np
  )

  add_entrypoint_object(
    pthread_join
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_join
  )

  add_entrypoint_object(
    pthread_key_create
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_key_create
  )

  add_entrypoint_object(
    pthread_key_delete
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_key_delete
  )

  add_entrypoint_object(
    pthread_mutex_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutex_destroy
  )

  add_entrypoint_object(
    pthread_mutex_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutex_init
  )

  add_entrypoint_object(
    pthread_mutex_lock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutex_lock
  )

  add_entrypoint_object(
    pthread_mutex_trylock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutex_trylock
  )

  add_entrypoint_object(
    pthread_mutex_unlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutex_unlock
  )

  add_entrypoint_object(
    pthread_mutexattr_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_destroy
  )

  add_entrypoint_object(
    pthread_mutexattr_getpshared
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_getpshared
  )

  add_entrypoint_object(
    pthread_mutexattr_getrobust
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_getrobust
  )

  add_entrypoint_object(
    pthread_mutexattr_gettype
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_gettype
  )

  add_entrypoint_object(
    pthread_mutexattr_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_init
  )

  add_entrypoint_object(
    pthread_mutexattr_setpshared
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_setpshared
  )

  add_entrypoint_object(
    pthread_mutexattr_setrobust
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_setrobust
  )

  add_entrypoint_object(
    pthread_mutexattr_settype
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_mutexattr_settype
  )

  add_entrypoint_object(
    pthread_once
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_once
  )

  add_entrypoint_object(
    pthread_rwlock_clockrdlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_clockrdlock
  )

  add_entrypoint_object(
    pthread_rwlock_clockwrlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_clockwrlock
  )

  add_entrypoint_object(
    pthread_rwlock_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_destroy
  )

  add_entrypoint_object(
    pthread_rwlock_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_init
  )

  add_entrypoint_object(
    pthread_rwlock_rdlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_rdlock
  )

  add_entrypoint_object(
    pthread_rwlock_timedrdlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_timedrdlock
  )

  add_entrypoint_object(
    pthread_rwlock_timedwrlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_timedwrlock
  )

  add_entrypoint_object(
    pthread_rwlock_tryrdlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_tryrdlock
  )

  add_entrypoint_object(
    pthread_rwlock_trywrlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_trywrlock
  )

  add_entrypoint_object(
    pthread_rwlock_unlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_unlock
  )

  add_entrypoint_object(
    pthread_rwlock_wrlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlock_wrlock
  )

  add_entrypoint_object(
    pthread_rwlockattr_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlockattr_destroy
  )

  add_entrypoint_object(
    pthread_rwlockattr_getkind_np
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlockattr_getkind_np
  )

  add_entrypoint_object(
    pthread_rwlockattr_getpshared
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlockattr_getpshared
  )

  add_entrypoint_object(
    pthread_rwlockattr_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlockattr_init
  )

  add_entrypoint_object(
    pthread_rwlockattr_setkind_np
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlockattr_setkind_np
  )

  add_entrypoint_object(
    pthread_rwlockattr_setpshared
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_rwlockattr_setpshared
  )

  add_entrypoint_object(
    pthread_self
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_self
  )

  add_entrypoint_object(
    pthread_setname_np
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_setname_np
  )

  add_entrypoint_object(
    pthread_setspecific
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_setspecific
  )

  add_entrypoint_object(
    pthread_spin_destroy
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_spin_destroy
  )

  add_entrypoint_object(
    pthread_spin_init
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_spin_init
  )

  add_entrypoint_object(
    pthread_spin_lock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_spin_lock
  )

  add_entrypoint_object(
    pthread_spin_trylock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_spin_trylock
  )

  add_entrypoint_object(
    pthread_spin_unlock
    ALIAS
    DEPENDS
      .${LIBC_TARGET_OS}.pthread_spin_unlock
  )

  if(LIBC_TARGET_OS STREQUAL "wasi")
    add_entrypoint_object(
      pthread_cancel
      ALIAS
      DEPENDS
        .wasi.pthread_cancel
    )
  endif()

  return()
endif()

add_entrypoint_object(
  pthread_attr_init
  SRCS
    pthread_attr_init.cpp
  HDRS
    pthread_attr_init.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_destroy
  SRCS
    pthread_attr_destroy.cpp
  HDRS
    pthread_attr_destroy.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_getdetachstate
  SRCS
    pthread_attr_getdetachstate.cpp
  HDRS
    pthread_attr_getdetachstate.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_setdetachstate
  SRCS
    pthread_attr_setdetachstate.cpp
  HDRS
    pthread_attr_setdetachstate.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_attr_getguardsize
  SRCS
    pthread_attr_getguardsize.cpp
  HDRS
    pthread_attr_getguardsize.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_setguardsize
  SRCS
    pthread_attr_setguardsize.cpp
  HDRS
    pthread_attr_setguardsize.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_attr_getschedparam
  SRCS
    pthread_attr_getschedparam.cpp
  HDRS
    pthread_attr_getschedparam.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_setschedparam
  SRCS
    pthread_attr_setschedparam.cpp
  HDRS
    pthread_attr_setschedparam.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_getstacksize
  SRCS
    pthread_attr_getstacksize.cpp
  HDRS
    pthread_attr_getstacksize.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_attr_setstacksize
  SRCS
    pthread_attr_setstacksize.cpp
  HDRS
    pthread_attr_setstacksize.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_attr_getstack
  SRCS
    pthread_attr_getstack.cpp
  HDRS
    pthread_attr_getstack.h
  DEPENDS
    libc.include.pthread
    libc.src.pthread.pthread_attr_getstacksize
)

add_entrypoint_object(
  pthread_attr_setstack
  SRCS
    pthread_attr_setstack.cpp
  HDRS
    pthread_attr_setstack.h
  DEPENDS
    libc.hdr.stdint_proxy
    libc.include.pthread
    libc.src.pthread.pthread_attr_setstacksize
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_condattr_destroy
  SRCS
    pthread_condattr_destroy.cpp
  HDRS
    pthread_condattr_destroy.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_condattr_getclock
  SRCS
    pthread_condattr_getclock.cpp
  HDRS
    pthread_condattr_getclock.h
  DEPENDS
    libc.include.pthread
    libc.include.sys_types
)

add_entrypoint_object(
  pthread_condattr_getpshared
  SRCS
    pthread_condattr_getpshared.cpp
  HDRS
    pthread_condattr_getpshared.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_condattr_init
  SRCS
    pthread_condattr_init.cpp
  HDRS
    pthread_condattr_init.h
  DEPENDS
    libc.include.pthread
    libc.include.time
)

add_entrypoint_object(
  pthread_condattr_setclock
  SRCS
    pthread_condattr_setclock.cpp
  HDRS
    pthread_condattr_setclock.h
  DEPENDS
    libc.include.pthread
    libc.include.sys_types
    libc.include.time
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_condattr_setpshared
  SRCS
    pthread_condattr_setpshared.cpp
  HDRS
    pthread_condattr_setpshared.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
)

add_header_library(
  pthread_cond_utils
  HDRS
    pthread_cond_utils.h
  DEPENDS
    libc.hdr.errno_macros
    libc.hdr.time_macros
    libc.include.llvm-libc-types.clockid_t
    libc.include.llvm-libc-types.pthread_cond_t
    libc.include.llvm-libc-types.pthread_mutex_t
    libc.include.llvm-libc-types.struct_timespec
    libc.src.__support.CPP.optional
    libc.src.__support.macros.null_check
    libc.src.__support.macros.optimization
    libc.src.__support.threads.CndVar
    libc.src.__support.threads.mutex
    libc.src.__support.time.abs_timeout
)

add_entrypoint_object(
  pthread_cond_broadcast
  SRCS
    pthread_cond_broadcast.cpp
  HDRS
    pthread_cond_broadcast.h
  DEPENDS
    .pthread_cond_utils
)

add_entrypoint_object(
  pthread_cond_clockwait
  SRCS
    pthread_cond_clockwait.cpp
  HDRS
    pthread_cond_clockwait.h
  DEPENDS
    .pthread_cond_utils
    libc.hdr.errno_macros
    libc.include.llvm-libc-types.clockid_t
    libc.include.llvm-libc-types.pthread_cond_t
    libc.include.llvm-libc-types.pthread_mutex_t
    libc.include.llvm-libc-types.struct_timespec
)

add_entrypoint_object(
  pthread_cond_destroy
  SRCS
    pthread_cond_destroy.cpp
  HDRS
    pthread_cond_destroy.h
  DEPENDS
    .pthread_cond_utils
)

add_entrypoint_object(
  pthread_cond_init
  SRCS
    pthread_cond_init.cpp
  HDRS
    pthread_cond_init.h
  DEPENDS
    libc.hdr.errno_macros
    libc.hdr.time_macros
    libc.include.llvm-libc-macros.pthread_macros
    libc.include.llvm-libc-types.pthread_cond_t
    libc.include.llvm-libc-types.pthread_condattr_t
    libc.src.__support.CPP.new
    libc.src.__support.macros.null_check
    libc.src.__support.threads.CndVar
)

add_entrypoint_object(
  pthread_cond_signal
  SRCS
    pthread_cond_signal.cpp
  HDRS
    pthread_cond_signal.h
  DEPENDS
    .pthread_cond_utils
)

add_entrypoint_object(
  pthread_cond_timedwait
  SRCS
    pthread_cond_timedwait.cpp
  HDRS
    pthread_cond_timedwait.h
  DEPENDS
    .pthread_cond_utils
    libc.include.llvm-libc-types.pthread_cond_t
    libc.include.llvm-libc-types.pthread_mutex_t
    libc.include.llvm-libc-types.struct_timespec
)

add_entrypoint_object(
  pthread_cond_wait
  SRCS
    pthread_cond_wait.cpp
  HDRS
    pthread_cond_wait.h
  DEPENDS
    .pthread_cond_utils
    libc.src.__support.CPP.optional
    libc.include.llvm-libc-types.pthread_cond_t
    libc.include.llvm-libc-types.pthread_mutex_t
)

add_header_library(
  pthread_mutexattr
  HDRS
    pthread_mutexattr.h
  DEPENDS
    libc.src.__support.common
    libc.include.pthread
)

add_entrypoint_object(
  pthread_mutexattr_init
  SRCS
    pthread_mutexattr_init.cpp
  HDRS
    pthread_mutexattr_init.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
)

add_entrypoint_object(
  pthread_mutexattr_gettype
  SRCS
    pthread_mutexattr_gettype.cpp
  HDRS
    pthread_mutexattr_gettype.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
)

add_entrypoint_object(
  pthread_mutexattr_settype
  SRCS
    pthread_mutexattr_settype.cpp
  HDRS
    pthread_mutexattr_settype.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_mutexattr_destroy
  SRCS
    pthread_mutexattr_destroy.cpp
  HDRS
    pthread_mutexattr_destroy.h
  DEPENDS
    .pthread_mutexattr
    libc.src.errno.errno
    libc.include.pthread
)

add_entrypoint_object(
  pthread_mutexattr_getrobust
  SRCS
    pthread_mutexattr_getrobust.cpp
  HDRS
    pthread_mutexattr_getrobust.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
)

add_entrypoint_object(
  pthread_mutexattr_setrobust
  SRCS
    pthread_mutexattr_setrobust.cpp
  HDRS
    pthread_mutexattr_setrobust.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_mutexattr_getpshared
  SRCS
    pthread_mutexattr_getpshared.cpp
  HDRS
    pthread_mutexattr_getpshared.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
)

add_entrypoint_object(
  pthread_mutexattr_setpshared
  SRCS
    pthread_mutexattr_setpshared.cpp
  HDRS
    pthread_mutexattr_setpshared.h
  DEPENDS
    .pthread_mutexattr
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_barrier_init
  SRCS
    pthread_barrier_init.cpp
  HDRS
    pthread_barrier_init.h
  DEPENDS
    libc.src.errno.errno
    libc.include.pthread
    libc.src.__support.threads.linux.barrier
)

add_entrypoint_object(
  pthread_barrier_destroy
  SRCS
    pthread_barrier_destroy.cpp
  HDRS
    pthread_barrier_destroy.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.linux.barrier
)

add_entrypoint_object(
  pthread_barrier_wait
  SRCS
    pthread_barrier_wait.cpp
  HDRS
    pthread_barrier_wait.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.linux.barrier
)

add_entrypoint_object(
  pthread_mutex_init
  SRCS
    pthread_mutex_init.cpp
  HDRS
    pthread_mutex_init.h
  DEPENDS
    .pthread_mutexattr
    libc.src.errno.errno
    libc.include.pthread
    libc.src.__support.threads.mutex
)

add_entrypoint_object(
  pthread_mutex_destroy
  SRCS
    pthread_mutex_destroy.cpp
  HDRS
    pthread_mutex_destroy.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.mutex
)

add_entrypoint_object(
  pthread_mutex_lock
  SRCS
    pthread_mutex_lock.cpp
  HDRS
    pthread_mutex_lock.h
  DEPENDS
    libc.hdr.errno_macros
    libc.include.pthread
    libc.src.__support.threads.mutex
)

add_entrypoint_object(
  pthread_mutex_trylock
  SRCS
    pthread_mutex_trylock.cpp
  HDRS
    pthread_mutex_trylock.h
  DEPENDS
    libc.hdr.errno_macros
    libc.include.pthread
    libc.src.__support.threads.mutex
)

add_entrypoint_object(
  pthread_mutex_unlock
  SRCS
    pthread_mutex_unlock.cpp
  HDRS
    pthread_mutex_unlock.h
  DEPENDS
    libc.hdr.errno_macros
    libc.include.pthread
    libc.src.__support.libc_assert
    libc.src.__support.threads.mutex
)

add_entrypoint_object(
  pthread_create
  SRCS
    pthread_create.cpp
  HDRS
    pthread_create.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
    libc.src.pthread.pthread_attr_destroy
    libc.src.pthread.pthread_attr_init
    libc.src.pthread.pthread_attr_getdetachstate
    libc.src.pthread.pthread_attr_getguardsize
    libc.src.pthread.pthread_attr_getstack
    libc.src.errno.errno
  COMPILE_OPTIONS
    ${libc_opt_high_flag}
    -fno-omit-frame-pointer
)

add_entrypoint_object(
  pthread_join
  SRCS
    pthread_join.cpp
  HDRS
    pthread_join.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_detach
  SRCS
    pthread_detach.cpp
  HDRS
    pthread_detach.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_equal
  SRCS
    pthread_equal.cpp
  HDRS
    pthread_equal.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_exit
  SRCS
    pthread_exit.cpp
  HDRS
    pthread_exit.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_self
  SRCS
    pthread_self.cpp
  HDRS
    pthread_self.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_setname_np
  SRCS
    pthread_setname_np.cpp
  HDRS
    pthread_setname_np.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.CPP.span
    libc.src.__support.CPP.stringstream
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_getname_np
  SRCS
    pthread_getname_np.cpp
  HDRS
    pthread_getname_np.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.CPP.span
    libc.src.__support.CPP.stringstream
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_getthreadid_np
  SRCS
    pthread_getthreadid_np.cpp
  HDRS
    pthread_getthreadid_np.h
  DEPENDS
    libc.include.llvm-libc-types.pthread_id_np_t
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_getunique_np
  SRCS
    pthread_getunique_np.cpp
  HDRS
    pthread_getunique_np.h
  DEPENDS
    libc.hdr.errno_macros
    libc.include.llvm-libc-types.pthread_id_np_t
    libc.include.llvm-libc-types.pthread_t
)

add_entrypoint_object(
  pthread_key_create
  SRCS
    pthread_key_create.cpp
  HDRS
    pthread_key_create.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_key_delete
  SRCS
    pthread_key_delete.cpp
  HDRS
    pthread_key_delete.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_getspecific
  SRCS
    pthread_getspecific.cpp
  HDRS
    pthread_getspecific.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
    libc.src.__support.threads.thread
)

add_entrypoint_object(
  pthread_setspecific
  SRCS
    pthread_setspecific.cpp
  HDRS
    pthread_setspecific.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.thread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_rwlockattr_destroy
  SRCS
    pthread_rwlockattr_destroy.cpp
  HDRS
    pthread_rwlockattr_destroy.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_rwlockattr_getkind_np
  SRCS
    pthread_rwlockattr_getkind_np.cpp
  HDRS
    pthread_rwlockattr_getkind_np.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_rwlockattr_getpshared
  SRCS
    pthread_rwlockattr_getpshared.cpp
  HDRS
    pthread_rwlockattr_getpshared.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_rwlockattr_init
  SRCS
    pthread_rwlockattr_init.cpp
  HDRS
    pthread_rwlockattr_init.h
  DEPENDS
    libc.include.pthread
)

add_entrypoint_object(
  pthread_rwlockattr_setkind_np
  SRCS
    pthread_rwlockattr_setkind_np.cpp
  HDRS
    pthread_rwlockattr_setkind_np.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_rwlockattr_setpshared
  SRCS
    pthread_rwlockattr_setpshared.cpp
  HDRS
    pthread_rwlockattr_setpshared.h
  DEPENDS
    libc.include.pthread
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_rwlock_init
  SRCS
    pthread_rwlock_init.cpp
  HDRS
    pthread_rwlock_init.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
    libc.src.__support.CPP.new
)

add_entrypoint_object(
  pthread_rwlock_tryrdlock
  SRCS
    pthread_rwlock_tryrdlock.cpp
  HDRS
    pthread_rwlock_tryrdlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_rwlock_trywrlock
  SRCS
    pthread_rwlock_trywrlock.cpp
  HDRS
    pthread_rwlock_trywrlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_rwlock_clockrdlock
  SRCS
    pthread_rwlock_clockrdlock.cpp
  HDRS
    pthread_rwlock_clockrdlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_rwlock_clockwrlock
  SRCS
    pthread_rwlock_clockwrlock.cpp
  HDRS
    pthread_rwlock_clockwrlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_rwlock_timedrdlock
  SRCS
    pthread_rwlock_timedrdlock.cpp
  HDRS
    pthread_rwlock_timedrdlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_rwlock_timedwrlock
  SRCS
    pthread_rwlock_timedwrlock.cpp
  HDRS
    pthread_rwlock_timedwrlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_rwlock_rdlock
  SRCS
    pthread_rwlock_rdlock.cpp
  HDRS
    pthread_rwlock_rdlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_rwlock_wrlock
  SRCS
    pthread_rwlock_wrlock.cpp
  HDRS
    pthread_rwlock_wrlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_rwlock_unlock
  SRCS
    pthread_rwlock_unlock.cpp
  HDRS
    pthread_rwlock_unlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
    libc.src.errno.errno
)

add_entrypoint_object(
  pthread_rwlock_destroy
  SRCS
    pthread_rwlock_destroy.cpp
  HDRS
    pthread_rwlock_destroy.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.unix_rwlock
)

add_entrypoint_object(
  pthread_spin_init
  SRCS
    pthread_spin_init.cpp
  HDRS
    pthread_spin_init.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.spin_lock
    libc.src.__support.threads.identifier
    libc.hdr.errno_macros
)

add_entrypoint_object(
  pthread_spin_destroy
  SRCS
    pthread_spin_destroy.cpp
  HDRS
    pthread_spin_destroy.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.spin_lock
    libc.src.__support.threads.identifier
    libc.hdr.errno_macros
)

add_entrypoint_object(
  pthread_spin_lock
  SRCS
    pthread_spin_lock.cpp
  HDRS
    pthread_spin_lock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.spin_lock
    libc.src.__support.threads.identifier
    libc.hdr.errno_macros
)

add_entrypoint_object(
  pthread_spin_trylock
  SRCS
    pthread_spin_trylock.cpp
  HDRS
    pthread_spin_trylock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.spin_lock
    libc.src.__support.threads.identifier
    libc.hdr.errno_macros
)

add_entrypoint_object(
  pthread_spin_unlock
  SRCS
    pthread_spin_unlock.cpp
  HDRS
    pthread_spin_unlock.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.spin_lock
    libc.src.__support.threads.identifier
    libc.hdr.errno_macros
)

add_entrypoint_object(
  pthread_once
  SRCS
    pthread_once.cpp
  HDRS
    pthread_once.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.callonce
)

add_entrypoint_object(
  pthread_atfork
  SRCS
    pthread_atfork.cpp
  HDRS
    pthread_atfork.h
  DEPENDS
    libc.include.pthread
    libc.src.__support.threads.fork_callbacks
    libc.src.errno.errno
)
