//===-- Portable fmemopen implementation ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/fmemopen.h"

#include "hdr/stdio_macros.h"
#include "hdr/types/cookie_io_functions_t.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/stdio/fopencookie.h"
#include "src/stdlib/free.h"
#include "src/stdlib/malloc.h"

namespace LIBC_NAMESPACE_DECL {
namespace {

struct MemoryCookie {
  char *buffer;
  size_t capacity;
  size_t length;
  size_t position;
  bool owns_buffer;
  bool append;
};

ssize_t memory_read(void *context, char *out, size_t count) {
  auto *cookie = static_cast<MemoryCookie *>(context);
  size_t available =
      cookie->position < cookie->length ? cookie->length - cookie->position : 0;
  if (count > available)
    count = available;
  __builtin_memcpy(out, cookie->buffer + cookie->position, count);
  cookie->position += count;
  return static_cast<ssize_t>(count);
}

ssize_t memory_write(void *context, const char *data, size_t count) {
  auto *cookie = static_cast<MemoryCookie *>(context);
  if (count == 0)
    return 0;
  if (cookie->append)
    cookie->position = cookie->length;
  size_t available = cookie->capacity - cookie->position;
  if (count > available)
    count = available;
  if (count == 0) {
    libc_errno = ENOSPC;
    return -1;
  }
  if (cookie->position > cookie->length)
    __builtin_memset(cookie->buffer + cookie->length, 0,
                     cookie->position - cookie->length);
  __builtin_memcpy(cookie->buffer + cookie->position, data, count);
  cookie->position += count;
  if (cookie->position > cookie->length)
    cookie->length = cookie->position;
  if (cookie->length < cookie->capacity)
    cookie->buffer[cookie->length] = '\0';
  return static_cast<ssize_t>(count);
}

int memory_seek(void *context, off64_t *offset, int whence) {
  auto *cookie = static_cast<MemoryCookie *>(context);
  off64_t base;
  switch (whence) {
  case SEEK_SET:
    base = 0;
    break;
  case SEEK_CUR:
    base = static_cast<off64_t>(cookie->position);
    break;
  case SEEK_END:
    base = static_cast<off64_t>(cookie->length);
    break;
  default:
    libc_errno = EINVAL;
    return -1;
  }
  if (*offset < -base ||
      *offset > static_cast<off64_t>(cookie->capacity) - base) {
    libc_errno = EINVAL;
    return -1;
  }
  cookie->position = static_cast<size_t>(base + *offset);
  *offset = static_cast<off64_t>(cookie->position);
  return 0;
}

int memory_close(void *context) {
  auto *cookie = static_cast<MemoryCookie *>(context);
  if (cookie->owns_buffer)
    free(cookie->buffer);
  free(cookie);
  return 0;
}

} // namespace

LLVM_LIBC_FUNCTION(::FILE *, fmemopen,
                   (void *buffer, size_t size, const char *mode)) {
  if (mode == nullptr || size == 0 ||
      (mode[0] != 'r' && mode[0] != 'w' && mode[0] != 'a')) {
    libc_errno = EINVAL;
    return nullptr;
  }
  auto *cookie = static_cast<MemoryCookie *>(malloc(sizeof(MemoryCookie)));
  if (cookie == nullptr)
    return nullptr;
  cookie->buffer = buffer == nullptr ? static_cast<char *>(malloc(size))
                                     : static_cast<char *>(buffer);
  if (cookie->buffer == nullptr) {
    free(cookie);
    return nullptr;
  }
  cookie->capacity = size;
  cookie->owns_buffer = buffer == nullptr;
  cookie->append = mode[0] == 'a';
  if (buffer == nullptr)
    __builtin_memset(cookie->buffer, 0, size);
  if (mode[0] == 'w') {
    cookie->length = 0;
    cookie->buffer[0] = '\0';
  } else if (cookie->append) {
    size_t length = 0;
    while (length < size && cookie->buffer[length] != '\0')
      ++length;
    cookie->length = length;
  } else {
    cookie->length = size;
  }
  cookie->position = cookie->append ? cookie->length : 0;
  cookie_io_functions_t ops = {memory_read, memory_write, memory_seek,
                               memory_close};
  ::FILE *stream = fopencookie(cookie, mode, ops);
  if (stream == nullptr)
    memory_close(cookie);
  return stream;
}

} // namespace LIBC_NAMESPACE_DECL
