//===-- epoll emulation over poll() for WASI ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// WASI has no epoll or eventfd facilities.  Epoll instances are emulated in
// userspace: each instance keeps a set of watched (fd, events) pairs and
// epoll_wait() blocks in poll(), which is itself built on poll_oneoff.
// Level-triggered semantics are preserved; EPOLLET is accepted but behaves
// like level-triggered, and EPOLLONESHOT entries are disabled after being
// reported until an EPOLL_CTL_MOD rearms them.
//
//===----------------------------------------------------------------------===//

#include "src/sys/epoll/wasi/epoll_state.h"

#include "hdr/errno_macros.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/libc_errno.h"
#include "src/poll/poll.h"

#include "llvm-libc-macros/poll-macros.h"

#include <string.h>
#include <sys/epoll.h>

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

namespace {

constexpr int MAX_EPOLL_INSTANCES = 8;
constexpr int MAX_EPOLL_FDS = 128;

// Synthetic descriptor base, far away from real WASI descriptor numbers.
constexpr int EPOLL_FD_BASE = 0x40000000;

struct WatchedFd {
  bool used;
  bool enabled;
  int fd;
  uint32_t events;
  uint64_t data;
};

struct EpollInstance {
  bool used;
  WatchedFd watched[MAX_EPOLL_FDS];
};

EpollInstance instances[MAX_EPOLL_INSTANCES];

int instance_index(int epfd) {
  if (epfd < EPOLL_FD_BASE ||
      epfd >= EPOLL_FD_BASE + MAX_EPOLL_INSTANCES)
    return -1;
  int index = epfd - EPOLL_FD_BASE;
  if (!instances[index].used)
    return -1;
  return index;
}

WatchedFd *find_entry(int index, int fd) {
  EpollInstance &inst = instances[index];
  for (int i = 0; i < MAX_EPOLL_FDS; ++i)
    if (inst.watched[i].used && inst.watched[i].fd == fd)
      return &inst.watched[i];
  return nullptr;
}

WatchedFd *find_free_entry(int index) {
  EpollInstance &inst = instances[index];
  for (int i = 0; i < MAX_EPOLL_FDS; ++i)
    if (!inst.watched[i].used)
      return &inst.watched[i];
  return nullptr;
}

unsigned short epoll_to_poll_events(uint32_t events) {
  unsigned short converted = 0;
  if (events & (EPOLLIN | EPOLLRDNORM | EPOLLRDBAND))
    converted |= POLLIN | POLLRDNORM;
  if (events & (EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND))
    converted |= POLLOUT | POLLWRNORM;
  if (events & EPOLLPRI)
    converted |= POLLPRI;
  return converted;
}

uint32_t poll_to_epoll_events(unsigned short revents) {
  uint32_t converted = 0;
  if (revents & (POLLIN | POLLPRI))
    converted |= EPOLLIN;
  if (revents & POLLPRI)
    converted |= EPOLLPRI;
  if (revents & POLLOUT)
    converted |= EPOLLOUT;
  if (revents & POLLERR)
    converted |= EPOLLERR;
  if (revents & POLLHUP)
    converted |= EPOLLHUP;
  if (revents & POLLRDNORM)
    converted |= EPOLLRDNORM;
  if (revents & POLLWRNORM)
    converted |= EPOLLWRNORM;
  if (revents & POLLNVAL)
    converted |= EPOLLERR;
  return converted;
}

} // namespace

int epoll_create_instance() {
  for (int i = 0; i < MAX_EPOLL_INSTANCES; ++i) {
    if (!instances[i].used) {
      instances[i].used = true;
      memset(instances[i].watched, 0, sizeof(instances[i].watched));
      return EPOLL_FD_BASE + i;
    }
  }
  libc_errno = ENFILE;
  return -1;
}

int epoll_release(int fd) {
  int index = instance_index(fd);
  if (index < 0)
    return 0;
  instances[index].used = false;
  memset(instances[index].watched, 0, sizeof(instances[index].watched));
  return 1;
}

int epoll_ctl_impl(int epfd, int op, int fd, struct epoll_event *event) {
  int index = instance_index(epfd);
  if (index < 0) {
    libc_errno = EBADF;
    return -1;
  }
  if (fd < 0) {
    libc_errno = EBADF;
    return -1;
  }
  if (op != EPOLL_CTL_ADD && op != EPOLL_CTL_MOD && op != EPOLL_CTL_DEL) {
    libc_errno = EINVAL;
    return -1;
  }
  if (op != EPOLL_CTL_DEL && event == nullptr) {
    libc_errno = EINVAL;
    return -1;
  }

  if (op == EPOLL_CTL_ADD) {
    if (fd == epfd) {
      libc_errno = EINVAL;
      return -1;
    }
    wasi::__wasi_fdstat_t stat;
    if (wasi::__wasi_fd_fdstat_get(fd, &stat) != wasi::__WASI_ERRNO_SUCCESS) {
      libc_errno = EBADF;
      return -1;
    }
  }

  WatchedFd *entry = find_entry(index, fd);
  if (op == EPOLL_CTL_ADD) {
    if (entry != nullptr) {
      libc_errno = EEXIST;
      return -1;
    }
    entry = find_free_entry(index);
    if (entry == nullptr) {
      libc_errno = ENOSPC;
      return -1;
    }
    entry->used = true;
    entry->fd = fd;
    entry->events = event->events;
    entry->enabled = true;
    entry->data = event->data.u64;
    return 0;
  }
  if (op == EPOLL_CTL_MOD) {
    if (entry == nullptr) {
      libc_errno = ENOENT;
      return -1;
    }
    entry->events = event->events;
    entry->enabled = true;
    entry->data = event->data.u64;
    return 0;
  }
  if (entry == nullptr) {
    libc_errno = ENOENT;
    return -1;
  }
  entry->used = false;
  return 0;
}

int epoll_wait_impl(int epfd, struct epoll_event *events, int maxevents,
                    int timeout_ms) {
  int index = instance_index(epfd);
  if (index < 0) {
    libc_errno = EBADF;
    return -1;
  }
  if (events == nullptr || maxevents <= 0) {
    libc_errno = EINVAL;
    return -1;
  }

  EpollInstance &inst = instances[index];

  struct pollfd pfds[MAX_EPOLL_FDS];
  int entry_of[MAX_EPOLL_FDS];
  nfds_t nfds = 0;
  for (int i = 0; i < MAX_EPOLL_FDS && nfds < MAX_EPOLL_FDS; ++i) {
    if (!inst.watched[i].used || !inst.watched[i].enabled)
      continue;
    unsigned short requested = epoll_to_poll_events(inst.watched[i].events);
    if (requested == 0)
      continue;
    pfds[nfds].fd = inst.watched[i].fd;
    pfds[nfds].events = requested;
    pfds[nfds].revents = 0;
    entry_of[nfds] = i;
    ++nfds;
  }

  if (nfds == 0) {
    if (timeout_ms > 0)
      poll(nullptr, 0, timeout_ms);
    return 0;
  }

  int ready = poll(pfds, nfds, timeout_ms);
  if (ready < 0)
    return -1;
  if (ready == 0)
    return 0;

  int reported = 0;
  for (nfds_t i = 0; i < nfds && reported < maxevents; ++i) {
    if (pfds[i].revents == 0)
      continue;
    WatchedFd &entry = inst.watched[entry_of[i]];
    uint32_t epoll_events = poll_to_epoll_events(pfds[i].revents);
    // Only report events the caller asked for, plus mandatory errors.
    uint32_t requested = entry.events;
    uint32_t out = epoll_events & (requested | EPOLLERR | EPOLLHUP);
    if (out == 0)
      continue;
    events[reported].events = out;
    events[reported].data.u64 = entry.data;
    ++reported;
    if (requested & EPOLLONESHOT)
      entry.enabled = false;
  }
  return reported;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL
