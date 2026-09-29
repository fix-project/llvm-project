//===-- WASI implementation of poll using poll_oneoff ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/poll/poll.h"

#include "hdr/func/free.h"
#include "hdr/func/malloc.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include <poll.h>

namespace LIBC_NAMESPACE_DECL {

namespace {

constexpr unsigned POLL_FLAG_READ =
    POLLIN | POLLPRI | POLLRDNORM | POLLRDBAND;
constexpr unsigned POLL_FLAG_WRITE = POLLOUT | POLLWRNORM | POLLWRBAND;

} // namespace

LLVM_LIBC_FUNCTION(int, poll, (pollfd *fds, nfds_t nfds, int timeout)) {
  using namespace wasi;

  // Subscription counts are represented by a 32-bit WASI size, and the
  // allocation below must not wrap for a caller-supplied nfds value.
  constexpr size_t BYTES_PER_FD =
      2 * (sizeof(__wasi_subscription_t) + sizeof(__wasi_event_t));
  if (nfds > (UINT32_MAX - 1) / 2 ||
      nfds > (SIZE_MAX - sizeof(__wasi_subscription_t) -
              sizeof(__wasi_event_t)) / BYTES_PER_FD) {
    libc_errno = EINVAL;
    return -1;
  }

  // At most two subscriptions per fd plus one clock subscription.
  __wasi_subscription_t stack_subs[65];
  __wasi_event_t stack_events[65];
  __wasi_subscription_t *subs = stack_subs;
  __wasi_event_t *events = stack_events;
  __wasi_size_t max_subs = nfds * 2 + 1;
  void *heap = nullptr;
  if (max_subs > 65) {
    heap = malloc(max_subs * (sizeof(__wasi_subscription_t) +
                              sizeof(__wasi_event_t)));
    if (heap == nullptr) {
      libc_errno = ENOMEM;
      return -1;
    }
    subs = static_cast<__wasi_subscription_t *>(heap);
    events = reinterpret_cast<__wasi_event_t *>(
        static_cast<char *>(heap) +
        max_subs * sizeof(__wasi_subscription_t));
  }

  __wasi_size_t nsubs = 0;
  int invalid_fds = 0;
  for (nfds_t i = 0; i < nfds; ++i) {
    fds[i].revents = 0;
    if (fds[i].fd < 0)
      continue;
    unsigned ev = static_cast<unsigned>(fds[i].events);
    // As entries are decomposed into separate read/write subscriptions,
    // POLLERR, POLLHUP and POLLNVAL cannot be detected if neither a read
    // nor a write event is requested.  Reject such entries, matching
    // wasi-libc.
    if ((ev & (POLL_FLAG_READ | POLL_FLAG_WRITE)) == 0) {
      if (heap != nullptr)
        free(heap);
      libc_errno = ENOSYS;
      return -1;
    }
    __wasi_fdstat_t stat;
    if (__wasi_fd_fdstat_get(fds[i].fd, &stat) == __WASI_ERRNO_BADF) {
      fds[i].revents = POLLNVAL;
      ++invalid_fds;
      continue;
    }
    if (ev & POLL_FLAG_READ) {
      subs[nsubs].userdata = i * 2;
      subs[nsubs].u.tag = __WASI_EVENTTYPE_FD_READ;
      subs[nsubs].u.u.fd_read.file_descriptor =
          static_cast<__wasi_fd_t>(fds[i].fd);
      ++nsubs;
    }
    if (ev & POLL_FLAG_WRITE) {
      subs[nsubs].userdata = i * 2 + 1;
      subs[nsubs].u.tag = __WASI_EVENTTYPE_FD_WRITE;
      subs[nsubs].u.u.fd_write.file_descriptor =
          static_cast<__wasi_fd_t>(fds[i].fd);
      ++nsubs;
    }
  }

  // An invalid descriptor makes poll return immediately, without waiting for
  // otherwise valid subscriptions.
  bool has_timeout = timeout >= 0 || invalid_fds != 0;
  if (has_timeout) {
    subs[nsubs].userdata = nfds * 2;
    subs[nsubs].u.tag = __WASI_EVENTTYPE_CLOCK;
    subs[nsubs].u.u.clock.id = __WASI_CLOCKID_MONOTONIC;
    subs[nsubs].u.u.clock.timeout = invalid_fds != 0
                                     ? 0
                                     : static_cast<__wasi_timestamp_t>(timeout) *
                                           UINT64_C(1000000);
    subs[nsubs].u.u.clock.precision = 0;
    subs[nsubs].u.u.clock.flags = 0;
    ++nsubs;
  }

  // WASI's poll_oneoff requires at least one subscription.  A poll with
  // nothing to wait for and no timeout cannot be supported (there is no
  // way for it to ever wake up), so report ENOTSUP, matching wasi-libc.
  if (nsubs == 0) {
    if (heap != nullptr)
      free(heap);
    libc_errno = ENOTSUP;
    return -1;
  }

  __wasi_size_t nevents = 0;
  __wasi_errno_t err = __wasi_poll_oneoff(subs, events, nsubs, &nevents);
  if (err != __WASI_ERRNO_SUCCESS) {
    if (heap != nullptr)
      free(heap);
    libc_errno = wasi_to_errno(err);
    return -1;
  }

  int ready = invalid_fds;
  bool timed_out = false;
  for (__wasi_size_t k = 0; k < nevents; ++k) {
    const __wasi_event_t &ev = events[k];
    if (ev.type == __WASI_EVENTTYPE_CLOCK) {
      timed_out = true;
      continue;
    }
    nfds_t i = static_cast<nfds_t>(ev.userdata / 2);
    bool is_write = (ev.userdata & 1) != 0;
    if (i >= nfds)
      continue;
    unsigned short revents = fds[i].revents;
    if (ev.error == __WASI_ERRNO_BADF) {
      revents |= POLLNVAL;
    } else if (ev.error == __WASI_ERRNO_PIPE) {
      revents |= POLLHUP;
    } else if (ev.error != __WASI_ERRNO_SUCCESS) {
      revents |= POLLERR;
    } else if (is_write) {
      revents |= POLLOUT | POLLWRNORM;
      if (ev.fd_readwrite.flags & __WASI_EVENTRWFLAGS_FD_READWRITE_HANGUP)
        revents |= POLLHUP;
    } else {
      revents |= POLLIN | POLLRDNORM;
      if (ev.fd_readwrite.flags & __WASI_EVENTRWFLAGS_FD_READWRITE_HANGUP)
        revents |= POLLHUP;
    }
    if (fds[i].revents == 0)
      ++ready;
    fds[i].revents = revents;
  }

  if (heap != nullptr)
    free(heap);

  // POLLHUP contradicts with POLLOUT, matching wasi-libc.
  for (nfds_t i = 0; i < nfds; ++i) {
    if (fds[i].revents & POLLHUP)
      fds[i].revents &= ~static_cast<unsigned short>(POLLOUT | POLLWRNORM);
  }

  // A fired timeout with no fd readiness is indistinguishable from the
  // "no events" case; poll_oneoff only returns when at least one event
  // fires, so a timeout event means the deadline elapsed first.
  if (timed_out && ready == 0)
    return 0;

  return ready;
}

} // namespace LIBC_NAMESPACE_DECL
