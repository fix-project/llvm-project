//===-- WASI implementation of select using poll --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/select/select.h"

#include "hdr/func/free.h"
#include "hdr/func/malloc.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/poll/poll.h"

#include <poll.h>
#include <sys/select.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, select,
                   (int nfds, fd_set *__restrict read_set,
                    fd_set *__restrict write_set, fd_set *__restrict error_set,
                    struct timeval *__restrict timeout)) {
  if (nfds < 0 || nfds > FD_SETSIZE ||
      (timeout != nullptr && (timeout->tv_sec < 0 || timeout->tv_usec < 0 ||
                              timeout->tv_usec >= 1000000))) {
    libc_errno = EINVAL;
    return -1;
  }

  if (error_set != nullptr)
    FD_ZERO(error_set); // WASI cannot report exceptional conditions.

  int timeout_ms = -1;
  if (timeout != nullptr) {
    constexpr long MAX_TIMEOUT_MS = 0x7fffffff;
    if (timeout->tv_sec >= MAX_TIMEOUT_MS / 1000)
      timeout_ms = static_cast<int>(MAX_TIMEOUT_MS);
    else
      timeout_ms = static_cast<int>(timeout->tv_sec * 1000 +
                                    (timeout->tv_usec + 999) / 1000);
  }

  if (nfds == 0)
    return LIBC_NAMESPACE::poll(nullptr, 0, timeout_ms);

  pollfd stack_fds[64];
  pollfd *pfds = stack_fds;
  void *heap = nullptr;
  if (nfds > 64) {
    heap = malloc(static_cast<size_t>(nfds) * sizeof(pollfd));
    if (heap == nullptr) {
      libc_errno = ENOMEM;
      return -1;
    }
    pfds = static_cast<pollfd *>(heap);
  }

  nfds_t count = 0;
  for (int fd = 0; fd < nfds; ++fd) {
    short events = 0;
    if (read_set != nullptr && FD_ISSET(fd, read_set))
      events |= POLLIN;
    if (write_set != nullptr && FD_ISSET(fd, write_set))
      events |= POLLOUT;
    if (events == 0)
      continue;
    pfds[count].fd = fd;
    pfds[count].events = events;
    pfds[count].revents = 0;
    ++count;
  }

  int result = LIBC_NAMESPACE::poll(pfds, count, timeout_ms);
  if (result < 0) {
    if (heap != nullptr)
      free(heap);
    return -1;
  }

  if (read_set != nullptr)
    FD_ZERO(read_set);
  if (write_set != nullptr)
    FD_ZERO(write_set);

  int ready = 0;
  for (nfds_t i = 0; i < count; ++i) {
    if (pfds[i].revents == 0)
      continue;
    ++ready;
    const int fd = pfds[i].fd;
    if (read_set != nullptr &&
        (pfds[i].revents & (POLLIN | POLLHUP | POLLERR)) != 0)
      FD_SET(fd, read_set);
    if (write_set != nullptr && (pfds[i].revents & (POLLOUT | POLLERR)) != 0)
      FD_SET(fd, write_set);
  }

  if (heap != nullptr)
    free(heap);
  return ready;
}

} // namespace LIBC_NAMESPACE_DECL
