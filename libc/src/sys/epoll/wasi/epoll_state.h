//===-- Shared epoll emulation state for WASI -------------------*-C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_EPOLL_WASI_EPOLL_STATE_H
#define LLVM_LIBC_SRC_SYS_EPOLL_WASI_EPOLL_STATE_H

#include "src/__support/macros/config.h"

#include <stdint.h>

struct epoll_event;

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// Creates a new epoll instance and returns its file-descriptor-like handle,
// or -1 with libc_errno set.
int epoll_create_instance();

// Releases the epoll instance owned by fd.  Returns 1 if fd was an epoll
// handle (and was released), otherwise 0.
int epoll_release(int fd);

// Implements epoll_ctl.  Returns 0 or -1 with libc_errno set.
int epoll_ctl_impl(int epfd, int op, int fd, struct epoll_event *event);

// Implements epoll_wait with a millisecond timeout (-1 for infinite).
// Returns the number of reported events or -1 with libc_errno set.
int epoll_wait_impl(int epfd, struct epoll_event *events, int maxevents,
                    int timeout_ms);

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SYS_EPOLL_WASI_EPOLL_STATE_H
