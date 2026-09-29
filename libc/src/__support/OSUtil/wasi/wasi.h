//===-- WASI preview1 syscall declarations --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Minimal declarations of the WASI snapshot preview1 host interface. The
/// functions are imported directly from the `wasi_snapshot_preview1` module,
/// so no external runtime library is required.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_WASI_H
#define LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_WASI_H

#include "hdr/errno_macros.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include <stdint.h>
#include <stddef.h>

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

using __wasi_errno_t = uint16_t;
using __wasi_size_t = uint32_t;
using __wasi_fd_t = int32_t;
using __wasi_timestamp_t = uint64_t;
using __wasi_whence_t = uint8_t;
using __wasi_clockid_t = uint32_t;
using __wasi_fdflags_t = uint16_t;
using __wasi_rights_t = uint64_t;
using __wasi_lookupflags_t = uint32_t;
using __wasi_oflags_t = uint16_t;
using __wasi_openflags_t = uint32_t;
using __wasi_filetype_t = uint8_t;
using __wasi_fstflags_t = uint16_t;
using __wasi_filesize_t = uint64_t;
using __wasi_filedelta_t = int64_t;
using __wasi_device_t = uint64_t;
using __wasi_linkcount_t = uint64_t;
using __wasi_inode_t = uint64_t;
using __wasi_dircookie_t = uint64_t;
using __wasi_advice_t = uint8_t;
using __wasi_signal_t = uint8_t;
using __wasi_userdata_t = uint64_t;
using __wasi_eventtype_t = uint8_t;
using __wasi_eventrwflags_t = uint16_t;
using __wasi_subclockflags_t = uint16_t;

// Error values as defined by the WASI ABI.
enum __wasi_errno : __wasi_errno_t {
  __WASI_ERRNO_SUCCESS = 0,
  __WASI_ERRNO_2BIG = 1,
  __WASI_ERRNO_ACCES = 2,
  __WASI_ERRNO_ADDRINUSE = 3,
  __WASI_ERRNO_ADDRNOTAVAIL = 4,
  __WASI_ERRNO_AFNOSUPPORT = 5,
  __WASI_ERRNO_AGAIN = 6,
  __WASI_ERRNO_ARGVMAX = 7,
  __WASI_ERRNO_BADF = 8,
  __WASI_ERRNO_BADMSG = 9,
  __WASI_ERRNO_BUSY = 10,
  __WASI_ERRNO_CANCELED = 11,
  __WASI_ERRNO_CHILD = 12,
  __WASI_ERRNO_CONNABORTED = 13,
  __WASI_ERRNO_CONNREFUSED = 14,
  __WASI_ERRNO_CONNRESET = 15,
  __WASI_ERRNO_DEADLK = 16,
  __WASI_ERRNO_DESTADDRREQ = 17,
  __WASI_ERRNO_DOM = 18,
  __WASI_ERRNO_DQUOT = 19,
  __WASI_ERRNO_EXIST = 20,
  __WASI_ERRNO_FAULT = 21,
  __WASI_ERRNO_FBIG = 22,
  __WASI_ERRNO_HOSTUNREACH = 23,
  __WASI_ERRNO_IDRM = 24,
  __WASI_ERRNO_ILSEQ = 25,
  __WASI_ERRNO_INPROGRESS = 26,
  __WASI_ERRNO_INTR = 27,
  __WASI_ERRNO_INVAL = 28,
  __WASI_ERRNO_IO = 29,
  __WASI_ERRNO_ISCONN = 30,
  __WASI_ERRNO_ISDIR = 31,
  __WASI_ERRNO_LOOP = 32,
  __WASI_ERRNO_MFILE = 33,
  __WASI_ERRNO_MLINKS = 34,
  __WASI_ERRNO_MSGSIZE = 35,
  __WASI_ERRNO_MULTIHOP = 36,
  __WASI_ERRNO_NAMETOOLONG = 37,
  __WASI_ERRNO_NETDOWN = 38,
  __WASI_ERRNO_NETRESET = 39,
  __WASI_ERRNO_NETUNREACH = 40,
  __WASI_ERRNO_NFILE = 41,
  __WASI_ERRNO_NOBUFS = 42,
  __WASI_ERRNO_NODEV = 43,
  __WASI_ERRNO_NOENT = 44,
  __WASI_ERRNO_NOEXEC = 45,
  __WASI_ERRNO_NOLCK = 46,
  __WASI_ERRNO_NOLINK = 47,
  __WASI_ERRNO_NOMEM = 48,
  __WASI_ERRNO_NOMSG = 49,
  __WASI_ERRNO_NOPROTOOPT = 50,
  __WASI_ERRNO_NOSPC = 51,
  __WASI_ERRNO_NOSYS = 52,
  __WASI_ERRNO_NOTCONN = 53,
  __WASI_ERRNO_NOTDIR = 54,
  __WASI_ERRNO_NOTEMPTY = 55,
  __WASI_ERRNO_NOTRECOVERABLE = 56,
  __WASI_ERRNO_NOTSOCK = 57,
  __WASI_ERRNO_NOTSUP = 58,
  __WASI_ERRNO_NOTTY = 59,
  __WASI_ERRNO_NXIO = 60,
  __WASI_ERRNO_OVERFLOW = 61,
  __WASI_ERRNO_OWNERDEAD = 62,
  __WASI_ERRNO_PERM = 63,
  __WASI_ERRNO_PIPE = 64,
  __WASI_ERRNO_PROTO = 65,
  __WASI_ERRNO_PROTONOSUPPORT = 66,
  __WASI_ERRNO_PROTOTYPE = 67,
  __WASI_ERRNO_RANGE = 68,
  __WASI_ERRNO_ROFS = 69,
  __WASI_ERRNO_SPIPE = 70,
  __WASI_ERRNO_SRCH = 71,
  __WASI_ERRNO_STALE = 72,
  __WASI_ERRNO_TIMEDOUT = 73,
  __WASI_ERRNO_TXTBSY = 74,
  __WASI_ERRNO_XDEV = 75,
  __WASI_ERRNO_NOTCAPABLE = 76,
};

// Convert a WASI error code to a POSIX error code. Unknown codes map to
// EIO so that failures are never reported as success.
LIBC_INLINE int wasi_to_errno(__wasi_errno_t err) {
  switch (err) {
  case __WASI_ERRNO_SUCCESS:
    return 0;
  case __WASI_ERRNO_2BIG:
    return E2BIG;
  case __WASI_ERRNO_ACCES:
    return EACCES;
  case __WASI_ERRNO_ADDRINUSE:
    return EADDRINUSE;
  case __WASI_ERRNO_ADDRNOTAVAIL:
    return EADDRNOTAVAIL;
  case __WASI_ERRNO_AFNOSUPPORT:
    return EAFNOSUPPORT;
  case __WASI_ERRNO_AGAIN:
    return EAGAIN;
  case __WASI_ERRNO_ARGVMAX:
    return E2BIG;
  case __WASI_ERRNO_BADF:
    return EBADF;
  case __WASI_ERRNO_BADMSG:
    return EBADMSG;
  case __WASI_ERRNO_BUSY:
    return EBUSY;
  case __WASI_ERRNO_CANCELED:
    return ECANCELED;
  case __WASI_ERRNO_CHILD:
    return ECHILD;
  case __WASI_ERRNO_CONNABORTED:
    return ECONNABORTED;
  case __WASI_ERRNO_CONNREFUSED:
    return ECONNREFUSED;
  case __WASI_ERRNO_CONNRESET:
    return ECONNRESET;
  case __WASI_ERRNO_DEADLK:
    return EDEADLK;
  case __WASI_ERRNO_DESTADDRREQ:
    return EDESTADDRREQ;
  case __WASI_ERRNO_DOM:
    return EDOM;
  case __WASI_ERRNO_DQUOT:
    return EDQUOT;
  case __WASI_ERRNO_EXIST:
    return EEXIST;
  case __WASI_ERRNO_FAULT:
    return EFAULT;
  case __WASI_ERRNO_FBIG:
    return EFBIG;
  case __WASI_ERRNO_HOSTUNREACH:
    return EHOSTUNREACH;
  case __WASI_ERRNO_IDRM:
    return EIDRM;
  case __WASI_ERRNO_ILSEQ:
    return EILSEQ;
  case __WASI_ERRNO_INPROGRESS:
    return EINPROGRESS;
  case __WASI_ERRNO_INTR:
    return EINTR;
  case __WASI_ERRNO_INVAL:
    return EINVAL;
  case __WASI_ERRNO_IO:
    return EIO;
  case __WASI_ERRNO_ISCONN:
    return EISCONN;
  case __WASI_ERRNO_ISDIR:
    return EISDIR;
  case __WASI_ERRNO_LOOP:
    return ELOOP;
  case __WASI_ERRNO_MFILE:
    return EMFILE;
  case __WASI_ERRNO_MLINKS:
    return EMLINK;
  case __WASI_ERRNO_MSGSIZE:
    return EMSGSIZE;
  case __WASI_ERRNO_MULTIHOP:
    return EMLINK;
  case __WASI_ERRNO_NAMETOOLONG:
    return ENAMETOOLONG;
  case __WASI_ERRNO_NETDOWN:
    return ENETDOWN;
  case __WASI_ERRNO_NETRESET:
    return ENETRESET;
  case __WASI_ERRNO_NETUNREACH:
    return ENETUNREACH;
  case __WASI_ERRNO_NFILE:
    return ENFILE;
  case __WASI_ERRNO_NOBUFS:
    return ENOBUFS;
  case __WASI_ERRNO_NODEV:
    return ENODEV;
  case __WASI_ERRNO_NOENT:
    return ENOENT;
  case __WASI_ERRNO_NOEXEC:
    return ENOEXEC;
  case __WASI_ERRNO_NOLCK:
    return ENOLCK;
  case __WASI_ERRNO_NOLINK:
    return ENOLINK;
  case __WASI_ERRNO_NOMEM:
    return ENOMEM;
  case __WASI_ERRNO_NOMSG:
    return ENOMSG;
  case __WASI_ERRNO_NOPROTOOPT:
    return ENOPROTOOPT;
  case __WASI_ERRNO_NOSPC:
    return ENOSPC;
  case __WASI_ERRNO_NOSYS:
    return ENOSYS;
  case __WASI_ERRNO_NOTCONN:
    return ENOTCONN;
  case __WASI_ERRNO_NOTDIR:
    return ENOTDIR;
  case __WASI_ERRNO_NOTEMPTY:
    return ENOTEMPTY;
  case __WASI_ERRNO_NOTRECOVERABLE:
    return EIO;
  case __WASI_ERRNO_NOTSOCK:
    return ENOTSOCK;
  case __WASI_ERRNO_NOTSUP:
    return ENOSYS;
  case __WASI_ERRNO_NOTTY:
    return ENOTTY;
  case __WASI_ERRNO_NXIO:
    return ENXIO;
  case __WASI_ERRNO_OVERFLOW:
    return EOVERFLOW;
  case __WASI_ERRNO_OWNERDEAD:
    return EOWNERDEAD;
  case __WASI_ERRNO_PERM:
    return EPERM;
  case __WASI_ERRNO_PIPE:
    return EPIPE;
  case __WASI_ERRNO_PROTO:
    return EPROTO;
  case __WASI_ERRNO_PROTONOSUPPORT:
    return EPROTONOSUPPORT;
  case __WASI_ERRNO_PROTOTYPE:
    return EPROTOTYPE;
  case __WASI_ERRNO_RANGE:
    return ERANGE;
  case __WASI_ERRNO_ROFS:
    return EROFS;
  case __WASI_ERRNO_SPIPE:
    return ESPIPE;
  case __WASI_ERRNO_SRCH:
    return ESRCH;
  case __WASI_ERRNO_STALE:
    return ESTALE;
  case __WASI_ERRNO_TIMEDOUT:
    return ETIMEDOUT;
  case __WASI_ERRNO_TXTBSY:
    return ETXTBSY;
  case __WASI_ERRNO_XDEV:
    return EXDEV;
  case __WASI_ERRNO_NOTCAPABLE:
    return EPERM;
  default:
    return EIO;
  }
}

// Returns true if [ptr, ptr + len) lies within the wasm linear memory.
// Used to translate invalid pointers into EFAULT instead of a wasm trap,
// since WASI syscalls abort on out-of-bounds memory references.
LIBC_INLINE bool wasm_ptr_valid(const volatile void *ptr, size_t len) {
  if (len == 0)
    return true;
  uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
  uintptr_t mem_bytes =
      static_cast<uintptr_t>(__builtin_wasm_memory_size(0)) * 65536u;
  return p != 0 && p + len >= p && p + len <= mem_bytes;
}

// Seek whence values (WASI and POSIX agree on these).
constexpr __wasi_whence_t __WASI_SEEK_SET = 0;
constexpr __wasi_whence_t __WASI_SEEK_CUR = 1;
constexpr __wasi_whence_t __WASI_SEEK_END = 2;

// Clock identifiers.
constexpr __wasi_clockid_t __WASI_CLOCKID_REALTIME = 0;
constexpr __wasi_clockid_t __WASI_CLOCKID_MONOTONIC = 1;
constexpr __wasi_clockid_t __WASI_CLOCKID_PROCESS_CPUTIME_ID = 2;
constexpr __wasi_clockid_t __WASI_CLOCKID_THREAD_CPUTIME_ID = 3;

// File descriptor flags.
constexpr __wasi_fdflags_t __WASI_FDFLAGS_APPEND = 1 << 0;
constexpr __wasi_fdflags_t __WASI_FDFLAGS_DSYNC = 1 << 1;
constexpr __wasi_fdflags_t __WASI_FDFLAGS_NONBLOCK = 1 << 2;
constexpr __wasi_fdflags_t __WASI_FDFLAGS_RSYNC = 1 << 3;
constexpr __wasi_fdflags_t __WASI_FDFLAGS_SYNC = 1 << 4;

// Open flags.
constexpr __wasi_oflags_t __WASI_OFLAGS_CREAT = 1 << 0;
constexpr __wasi_oflags_t __WASI_OFLAGS_DIRECTORY = 1 << 1;
constexpr __wasi_oflags_t __WASI_OFLAGS_EXCL = 1 << 2;
constexpr __wasi_oflags_t __WASI_OFLAGS_TRUNC = 1 << 3;

// Lookup flags.
constexpr __wasi_lookupflags_t __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW = 1 << 0;

// File type.
constexpr __wasi_filetype_t __WASI_FILETYPE_UNKNOWN = 0;
constexpr __wasi_filetype_t __WASI_FILETYPE_BLOCK_DEVICE = 1;
constexpr __wasi_filetype_t __WASI_FILETYPE_CHARACTER_DEVICE = 2;
constexpr __wasi_filetype_t __WASI_FILETYPE_DIRECTORY = 3;
constexpr __wasi_filetype_t __WASI_FILETYPE_REGULAR_FILE = 4;
constexpr __wasi_filetype_t __WASI_FILETYPE_SOCKET_DGRAM = 5;
constexpr __wasi_filetype_t __WASI_FILETYPE_SOCKET_STREAM = 6;
constexpr __wasi_filetype_t __WASI_FILETYPE_SYMBOLIC_LINK = 7;

// Filestat set flags.
constexpr __wasi_fstflags_t __WASI_FILESTAT_SET_ATIM = 1 << 0;
constexpr __wasi_fstflags_t __WASI_FILESTAT_SET_ATIM_NOW = 1 << 1;
constexpr __wasi_fstflags_t __WASI_FILESTAT_SET_MTIM = 1 << 2;
constexpr __wasi_fstflags_t __WASI_FILESTAT_SET_MTIM_NOW = 1 << 3;

constexpr __wasi_timestamp_t __WASI_FTIMES_SPECIAL_NOW = ~(__wasi_timestamp_t)0;
constexpr __wasi_timestamp_t __WASI_FTIMES_SPECIAL_OMIT =
    ~(__wasi_timestamp_t)0 - 1;

// Rights.
constexpr __wasi_rights_t __WASI_RIGHT_FD_DATASYNC = 1 << 0;
constexpr __wasi_rights_t __WASI_RIGHT_FD_READ = 1 << 1;
constexpr __wasi_rights_t __WASI_RIGHT_FD_SEEK = 1 << 2;
constexpr __wasi_rights_t __WASI_RIGHT_FD_FDSTAT_SET_FLAGS = 1 << 3;
constexpr __wasi_rights_t __WASI_RIGHT_FD_SYNC = 1 << 4;
constexpr __wasi_rights_t __WASI_RIGHT_FD_TELL = 1 << 5;
constexpr __wasi_rights_t __WASI_RIGHT_FD_WRITE = 1 << 6;
constexpr __wasi_rights_t __WASI_RIGHT_FD_ADVISE = 1 << 7;
constexpr __wasi_rights_t __WASI_RIGHT_FD_ALLOCATE = 1 << 8;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_CREATE_DIRECTORY = 1 << 9;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_CREATE_FILE = 1 << 10;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_LINK_SOURCE = 1 << 11;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_LINK_TARGET = 1 << 12;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_OPEN = 1 << 13;
constexpr __wasi_rights_t __WASI_RIGHT_FD_READDIR = 1 << 14;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_READLINK = 1 << 15;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_RENAME_SOURCE = 1 << 16;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_RENAME_TARGET = 1 << 17;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_FILESTAT_GET = 1 << 18;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_FILESTAT_SET_SIZE = 1 << 19;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_FILESTAT_SET_TIMES = 1 << 20;
constexpr __wasi_rights_t __WASI_RIGHT_FD_FILESTAT_GET = 1 << 21;
constexpr __wasi_rights_t __WASI_RIGHT_FD_FILESTAT_SET_SIZE = 1 << 22;
constexpr __wasi_rights_t __WASI_RIGHT_FD_FILESTAT_SET_TIMES = 1 << 23;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_SYMLINK = 1 << 24;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_REMOVE_DIRECTORY = 1 << 25;
constexpr __wasi_rights_t __WASI_RIGHT_PATH_UNLINK_FILE = 1 << 26;
constexpr __wasi_rights_t __WASI_RIGHT_POLL_FD_READWRITE = 1 << 27;
constexpr __wasi_rights_t __WASI_RIGHT_SOCK_SHUTDOWN = 1 << 28;
constexpr __wasi_rights_t __WASI_RIGHT_SOCK_ACCEPT = 1 << 29;

// Event types for poll_oneoff subscriptions and events.
constexpr __wasi_eventtype_t __WASI_EVENTTYPE_CLOCK = 0;
constexpr __wasi_eventtype_t __WASI_EVENTTYPE_FD_READ = 1;
constexpr __wasi_eventtype_t __WASI_EVENTTYPE_FD_WRITE = 2;

// Event flags reported for fd_readwrite events.
constexpr __wasi_eventrwflags_t __WASI_EVENTRWFLAGS_FD_READWRITE_HANGUP =
    1 << 0;

// Subscription clock flags.
constexpr __wasi_subclockflags_t
    __WASI_SUBCLOCKFLAGS_SUBSCRIPTION_CLOCK_ABSTIME = 1 << 0;

struct __wasi_iovec_t {
  void *buf;
  __wasi_size_t buf_len;
};

struct __wasi_ciovec_t {
  const void *buf;
  __wasi_size_t buf_len;
};

struct __wasi_fdstat_t {
  __wasi_filetype_t fs_filetype;
  __wasi_fdflags_t fs_flags;
  __wasi_rights_t fs_rights_base;
  __wasi_rights_t fs_rights_inheriting;
};

struct __wasi_filestat_t {
  __wasi_device_t st_dev;
  __wasi_inode_t st_ino;
  __wasi_filetype_t st_filetype;
  __wasi_linkcount_t st_nlink;
  __wasi_filesize_t st_size;
  __wasi_timestamp_t st_atim;
  __wasi_timestamp_t st_mtim;
  __wasi_timestamp_t st_ctim;
};

struct __wasi_dirent_t {
  __wasi_dircookie_t d_next;
  __wasi_inode_t d_ino;
  __wasi_size_t d_namlen;
  __wasi_filetype_t d_type;
};

// Preopened directory information. `pr_type` is 0 (PREOPENTYPE_DIR) for
// directories; `pr_name_len` is the length of the path returned by
// `fd_prestat_dir_name`.
struct __wasi_prestat_t {
  uint8_t pr_type;
  uint8_t padding[3];
  __wasi_size_t pr_name_len;
};

constexpr uint8_t __WASI_PREOPENTYPE_DIR = 0;

// Polling subscriptions and events (poll_oneoff). The layouts and sizes
// match the WASI preview1 ABI:
//   subscription_clock_t:        size 32, align 8
//   subscription_t:              size 48, align 8
//   event_fd_readwrite_t:        size 16, align 8
//   event_t:                     size 32, align 8
struct __wasi_subscription_clock_t {
  __wasi_clockid_t id;
  __wasi_timestamp_t timeout;
  __wasi_timestamp_t precision;
  __wasi_subclockflags_t flags;
};

struct __wasi_subscription_fd_readwrite_t {
  __wasi_fd_t file_descriptor;
};

union __wasi_subscription_u_u_t {
  __wasi_subscription_clock_t clock;
  __wasi_subscription_fd_readwrite_t fd_read;
  __wasi_subscription_fd_readwrite_t fd_write;
};

struct __wasi_subscription_u_t {
  uint8_t tag;
  __wasi_subscription_u_u_t u;
};

struct __wasi_subscription_t {
  __wasi_userdata_t userdata;
  __wasi_subscription_u_t u;
};

struct __wasi_event_fd_readwrite_t {
  __wasi_filesize_t nbytes;
  __wasi_eventrwflags_t flags;
};

struct __wasi_event_t {
  __wasi_userdata_t userdata;
  __wasi_errno_t error;
  __wasi_eventtype_t type;
  __wasi_event_fd_readwrite_t fd_readwrite;
};

// Standard file descriptors.
constexpr __wasi_fd_t __WASI_STDIN_FILENO = 0;
constexpr __wasi_fd_t __WASI_STDOUT_FILENO = 1;
constexpr __wasi_fd_t __WASI_STDERR_FILENO = 2;

// NOTE: AT_FDCWD is defined in hdr/fcntl_macros.h (value -2, matching
// wasi-libc). Code that needs it must include that header.

#define WASI_IMPORT(name)                                                      \
  __attribute__((import_module("wasi_snapshot_preview1"),                    \
                 import_name(name)))

extern "C" {

WASI_IMPORT("proc_exit")
[[noreturn]] void __wasi_proc_exit(__wasi_errno_t rval);

WASI_IMPORT("fd_write")
__wasi_errno_t __wasi_fd_write(__wasi_fd_t fd, const __wasi_ciovec_t *iovs,
                               size_t iovs_len, __wasi_size_t *nwritten);

WASI_IMPORT("fd_read")
__wasi_errno_t __wasi_fd_read(__wasi_fd_t fd, const __wasi_iovec_t *iovs,
                              size_t iovs_len, __wasi_size_t *nread);

WASI_IMPORT("fd_close")
__wasi_errno_t __wasi_fd_close(__wasi_fd_t fd);

WASI_IMPORT("fd_seek")
__wasi_errno_t __wasi_fd_seek(__wasi_fd_t fd, __wasi_filedelta_t offset,
                              __wasi_whence_t whence,
                              __wasi_filesize_t *newoffset);

WASI_IMPORT("fd_sync")
__wasi_errno_t __wasi_fd_sync(__wasi_fd_t fd);

WASI_IMPORT("fd_fdstat_get")
__wasi_errno_t __wasi_fd_fdstat_get(__wasi_fd_t fd, __wasi_fdstat_t *buf);

WASI_IMPORT("fd_fdstat_set_flags")
__wasi_errno_t __wasi_fd_fdstat_set_flags(__wasi_fd_t fd,
                                          __wasi_fdflags_t flags);

WASI_IMPORT("fd_filestat_get")
__wasi_errno_t __wasi_fd_filestat_get(__wasi_fd_t fd, __wasi_filestat_t *buf);

WASI_IMPORT("fd_filestat_set_size")
__wasi_errno_t __wasi_fd_filestat_set_size(__wasi_fd_t fd,
                                           __wasi_filesize_t size);

WASI_IMPORT("fd_filestat_set_times")
__wasi_errno_t __wasi_fd_filestat_set_times(__wasi_fd_t fd,
                                            __wasi_timestamp_t atim,
                                            __wasi_timestamp_t mtim,
                                            __wasi_fstflags_t fst_flags);

WASI_IMPORT("fd_readdir")
__wasi_errno_t __wasi_fd_readdir(__wasi_fd_t fd, uint8_t *buf,
                                 __wasi_size_t buf_len,
                                 __wasi_dircookie_t cookie,
                                 __wasi_size_t *buf_used);

WASI_IMPORT("fd_prestat_get")
__wasi_errno_t __wasi_fd_prestat_get(__wasi_fd_t fd, __wasi_prestat_t *buf);

WASI_IMPORT("fd_prestat_dir_name")
__wasi_errno_t __wasi_fd_prestat_dir_name(__wasi_fd_t fd, char *path,
                                          __wasi_size_t path_len);

WASI_IMPORT("path_open")
__wasi_errno_t __wasi_path_open(__wasi_fd_t dirfd,
                                __wasi_lookupflags_t dirflags, const char *path,
                                size_t path_len, __wasi_oflags_t oflags,
                                __wasi_rights_t fs_rights_base,
                                __wasi_rights_t fs_rights_inheriting,
                                __wasi_fdflags_t fdflags, __wasi_fd_t *opened_fd);

WASI_IMPORT("path_filestat_get")
__wasi_errno_t __wasi_path_filestat_get(__wasi_fd_t dirfd,
                                        __wasi_lookupflags_t flags,
                                        const char *path, size_t path_len,
                                        __wasi_filestat_t *buf);

WASI_IMPORT("path_filestat_set_times")
__wasi_errno_t __wasi_path_filestat_set_times(
    __wasi_fd_t dirfd, __wasi_lookupflags_t flags, const char *path,
    size_t path_len, __wasi_timestamp_t atim, __wasi_timestamp_t mtim,
    __wasi_fstflags_t fst_flags);

WASI_IMPORT("path_create_directory")
__wasi_errno_t __wasi_path_create_directory(__wasi_fd_t dirfd, const char *path,
                                            size_t path_len);

WASI_IMPORT("path_remove_directory")
__wasi_errno_t __wasi_path_remove_directory(__wasi_fd_t dirfd, const char *path,
                                            size_t path_len);

WASI_IMPORT("path_unlink_file")
__wasi_errno_t __wasi_path_unlink_file(__wasi_fd_t dirfd, const char *path,
                                       size_t path_len);

WASI_IMPORT("path_rename")
__wasi_errno_t __wasi_path_rename(__wasi_fd_t old_dirfd, const char *old_path,
                                  size_t old_path_len, __wasi_fd_t new_dirfd,
                                  const char *new_path, size_t new_path_len);

WASI_IMPORT("clock_res_get")
__wasi_errno_t __wasi_clock_res_get(__wasi_clockid_t id,
                                    __wasi_timestamp_t *resolution);

WASI_IMPORT("clock_time_get")
__wasi_errno_t __wasi_clock_time_get(__wasi_clockid_t id,
                                     __wasi_timestamp_t precision,
                                     __wasi_timestamp_t *resolution);

WASI_IMPORT("random_get")
__wasi_errno_t __wasi_random_get(uint8_t *buf, __wasi_size_t buf_len);

WASI_IMPORT("sched_yield")
__wasi_errno_t __wasi_sched_yield(void);

WASI_IMPORT("args_sizes_get")
__wasi_errno_t __wasi_args_sizes_get(__wasi_size_t *argc,
                                     __wasi_size_t *argv_buf_size);

WASI_IMPORT("args_get")
__wasi_errno_t __wasi_args_get(char ***argv, char *argv_buf);

WASI_IMPORT("environ_sizes_get")
__wasi_errno_t __wasi_environ_sizes_get(__wasi_size_t *environ_count,
                                        __wasi_size_t *environ_buf_size);

WASI_IMPORT("environ_get")
__wasi_errno_t __wasi_environ_get(char ***environ, char *environ_buf);

WASI_IMPORT("fd_pread")
__wasi_errno_t __wasi_fd_pread(__wasi_fd_t fd, const __wasi_iovec_t *iovs,
                               size_t iovs_len, __wasi_filesize_t offset,
                               __wasi_size_t *nread);

WASI_IMPORT("fd_pwrite")
__wasi_errno_t __wasi_fd_pwrite(__wasi_fd_t fd, const __wasi_ciovec_t *iovs,
                                size_t iovs_len, __wasi_filesize_t offset,
                                __wasi_size_t *nwritten);

WASI_IMPORT("fd_datasync")
__wasi_errno_t __wasi_fd_datasync(__wasi_fd_t fd);

WASI_IMPORT("fd_renumber")
__wasi_errno_t __wasi_fd_renumber(__wasi_fd_t fd, __wasi_fd_t to);

WASI_IMPORT("fd_advise")
__wasi_errno_t __wasi_fd_advise(__wasi_fd_t fd, __wasi_filesize_t offset,
                                __wasi_filesize_t length,
                                __wasi_advice_t advice);

WASI_IMPORT("fd_allocate")
__wasi_errno_t __wasi_fd_allocate(__wasi_fd_t fd, __wasi_filesize_t offset,
                                  __wasi_filesize_t length);

WASI_IMPORT("proc_raise")
__wasi_errno_t __wasi_proc_raise(__wasi_signal_t sig);

WASI_IMPORT("path_link")
__wasi_errno_t __wasi_path_link(__wasi_fd_t old_fd,
                                __wasi_lookupflags_t old_flags,
                                const char *old_path, size_t old_path_len,
                                __wasi_fd_t new_fd, const char *new_path,
                                size_t new_path_len);

WASI_IMPORT("path_symlink")
__wasi_errno_t __wasi_path_symlink(const char *old_path, size_t old_path_len,
                                   __wasi_fd_t fd, const char *new_path,
                                   size_t new_path_len);

WASI_IMPORT("path_readlink")
__wasi_errno_t __wasi_path_readlink(__wasi_fd_t fd, const char *path,
                                    size_t path_len, char *buf,
                                    size_t buf_len, __wasi_size_t *buf_used);

WASI_IMPORT("poll_oneoff")
__wasi_errno_t __wasi_poll_oneoff(const __wasi_subscription_t *in,
                                  __wasi_event_t *out,
                                  __wasi_size_t nsubscriptions,
                                  __wasi_size_t *nevents);

} // extern "C"

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_WASI_H
