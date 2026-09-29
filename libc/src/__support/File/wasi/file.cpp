//===--- Implementation of the WASI specialization of File ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "file.h"

#include "hdr/stdio_macros.h"
#include "hdr/types/off_t.h"
#include "src/__support/CPP/new.h"
#include "src/__support/File/file.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/alloc-checker.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

using namespace wasi;

namespace {
size_t str_len(const char *s) {
  size_t n = 0;
  while (s[n] != '\0')
    ++n;
  return n;
}
} // namespace

FileIOResult wasi_file_write(File *f, const void *data, size_t size) {
  auto *wf = reinterpret_cast<WasiFile *>(f);
  wasi::__wasi_ciovec_t iov = {const_cast<void *>(data),
                               static_cast<wasi::__wasi_size_t>(size)};
  wasi::__wasi_size_t nwritten = 0;
  __wasi_errno_t err =
      wasi::__wasi_fd_write(static_cast<wasi::__wasi_fd_t>(wf->get_fd()), &iov,
                            1, &nwritten);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return {0, -wasi::wasi_to_errno(err)};
  return nwritten;
}

FileIOResult wasi_file_read(File *f, void *buf, size_t size) {
  auto *wf = reinterpret_cast<WasiFile *>(f);
  wasi::__wasi_iovec_t iov = {buf, static_cast<wasi::__wasi_size_t>(size)};
  wasi::__wasi_size_t nread = 0;
  __wasi_errno_t err = wasi::__wasi_fd_read(
      static_cast<wasi::__wasi_fd_t>(wf->get_fd()), &iov, 1, &nread);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return {0, -wasi::wasi_to_errno(err)};
  return nread;
}

ErrorOr<off_t> wasi_file_seek(File *f, off_t offset, int whence) {
  auto *wf = reinterpret_cast<WasiFile *>(f);
  wasi::__wasi_whence_t wasi_whence;
  switch (whence) {
  case SEEK_SET:
    wasi_whence = wasi::__WASI_SEEK_SET;
    break;
  case SEEK_CUR:
    wasi_whence = wasi::__WASI_SEEK_CUR;
    break;
  case SEEK_END:
    wasi_whence = wasi::__WASI_SEEK_END;
    break;
  default:
    return Error(EINVAL);
  }
  wasi::__wasi_filesize_t newoffset = 0;
  __wasi_errno_t err = wasi::__wasi_fd_seek(
      static_cast<wasi::__wasi_fd_t>(wf->get_fd()),
      static_cast<wasi::__wasi_filedelta_t>(offset), wasi_whence, &newoffset);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return Error(wasi::wasi_to_errno(err));
  return static_cast<off_t>(newoffset);
}

int wasi_file_close(File *f) {
  File::remove_file(f);
  auto *wf = reinterpret_cast<WasiFile *>(f);
  __wasi_errno_t err =
      wasi::__wasi_fd_close(static_cast<wasi::__wasi_fd_t>(wf->get_fd()));
  if (wf->is_heap_allocated())
    delete wf;
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return wasi::wasi_to_errno(err);
  return 0;
}

ErrorOr<File *> openfile(const char *path, const char *mode) {
  using ModeFlags = File::ModeFlags;
  auto modeflags = File::mode_flags(mode);
  if (modeflags == 0)
    return Error(EINVAL);

  using OpenMode = File::OpenMode;
  using CreateType = File::CreateType;
  bool readable = (modeflags & ModeFlags(OpenMode::READ)) ||
                  (modeflags & ModeFlags(OpenMode::PLUS));
  bool writable =
      (modeflags & ModeFlags(OpenMode::WRITE)) ||
      (modeflags & ModeFlags(OpenMode::APPEND)) ||
      (modeflags & ModeFlags(OpenMode::PLUS));

  __wasi_oflags_t oflags = 0;
  if ((modeflags & ModeFlags(OpenMode::WRITE)) ||
      (modeflags & ModeFlags(OpenMode::APPEND)))
    oflags |= wasi::__WASI_OFLAGS_CREAT;
  if (modeflags & ModeFlags(OpenMode::WRITE))
    oflags |= wasi::__WASI_OFLAGS_TRUNC;
  if (modeflags & ModeFlags(CreateType::EXCLUSIVE))
    oflags |= wasi::__WASI_OFLAGS_EXCL;

  __wasi_fdflags_t fdflags = 0;
  if (modeflags & ModeFlags(OpenMode::APPEND))
    fdflags |= wasi::__WASI_FDFLAGS_APPEND;

  __wasi_rights_t rights = 0;
  if (readable)
    rights |= wasi::__WASI_RIGHT_FD_READ | wasi::__WASI_RIGHT_FD_SEEK |
              wasi::__WASI_RIGHT_FD_TELL | wasi::__WASI_RIGHT_FD_FILESTAT_GET;
  if (writable)
    rights |= wasi::__WASI_RIGHT_FD_WRITE | wasi::__WASI_RIGHT_FD_SEEK |
              wasi::__WASI_RIGHT_FD_FILESTAT_SET_SIZE;

  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(path, buf, sizeof(buf));
  if (!resolved.has_value())
    return Error(resolved.error());

  __wasi_fd_t fd;
  size_t rel_len = str_len(resolved->path);
  __wasi_errno_t err = wasi::__wasi_path_open(
      resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      rel_len, oflags, rights, rights, fdflags, &fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return Error(wasi::wasi_to_errno(err));

  uint8_t *buffer;
  {
    AllocChecker ac;
    buffer = new (ac) uint8_t[File::DEFAULT_BUFFER_SIZE];
    if (!ac) {
      wasi::__wasi_fd_close(fd);
      return Error(ENOMEM);
    }
  }
  AllocChecker ac;
  auto *file =
      new (ac) WasiFile(fd, buffer, File::DEFAULT_BUFFER_SIZE, _IOFBF, true,
                        modeflags);
  if (!ac) {
    wasi::__wasi_fd_close(fd);
    return Error(ENOMEM);
  }
  File::add_file(file);
  return file;
}

ErrorOr<WasiFile *> create_file_from_fd(int fd, const char *mode) {
  using ModeFlags = File::ModeFlags;
  ModeFlags modeflags = File::mode_flags(mode);
  if (modeflags == 0)
    return Error(EINVAL);

  // Verify the descriptor is valid.
  __wasi_fdstat_t fdstat;
  __wasi_errno_t err = wasi::__wasi_fd_fdstat_get(
      static_cast<wasi::__wasi_fd_t>(fd), &fdstat);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return Error(wasi::wasi_to_errno(err));

  uint8_t *buffer;
  {
    AllocChecker ac;
    buffer = new (ac) uint8_t[File::DEFAULT_BUFFER_SIZE];
    if (!ac)
      return Error(ENOMEM);
  }
  AllocChecker ac;
  auto *file =
      new (ac) WasiFile(fd, buffer, File::DEFAULT_BUFFER_SIZE, _IOFBF, true,
                        modeflags);
  if (!ac)
    return Error(ENOMEM);
  File::add_file(file);
  return file;
}

int get_fileno(File *f) {
  auto *wf = reinterpret_cast<WasiFile *>(f);
  return wf->get_fd();
}

} // namespace LIBC_NAMESPACE_DECL
