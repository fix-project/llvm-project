add_entrypoint_object(
  iconv
  SRCS
    iconv.cpp
  HDRS
    iconv.h
  DEPENDS
    libc.include.iconv
    libc.src.__support.common
    libc.src.__support.libc_errno
    libc.src.stdlib.malloc
    libc.src.stdlib.free
)

foreach(entrypoint iconv_open iconv_close)
  add_entrypoint_object(
    ${entrypoint}
    ALIAS
    DEPENDS
      .iconv
  )
endforeach()
