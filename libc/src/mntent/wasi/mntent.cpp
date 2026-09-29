#include "src/mntent/mntent.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/stdio/fopencookie.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace LIBC_NAMESPACE_DECL {
namespace {
constexpr size_t MAX_MOUNT_LINE = 4 * 4096 + 64;

struct MountCookie {
  int next_fd;
  size_t used;
  size_t position;
  char line[MAX_MOUNT_LINE];
};

bool append_char(MountCookie *cookie, char c) {
  if (cookie->used + 1 >= sizeof(cookie->line))
    return false;
  cookie->line[cookie->used++] = c;
  return true;
}

bool append_text(MountCookie *cookie, const char *text) {
  for (; *text; ++text)
    if (!append_char(cookie, *text))
      return false;
  return true;
}

bool append_number(MountCookie *cookie, unsigned value) {
  char digits[16];
  size_t length = 0;
  do {
    digits[length++] = static_cast<char>('0' + value % 10);
    value /= 10;
  } while (value);
  while (length)
    if (!append_char(cookie, digits[--length]))
      return false;
  return true;
}

bool append_mount_path(MountCookie *cookie, const char *name, size_t length) {
  if (length == 0 || (length == 1 && name[0] == '.'))
    return append_char(cookie, '/');
  if (length >= 2 && name[0] == '.' && name[1] == '/') {
    name += 2;
    length -= 2;
  }
  while (length > 1 && name[length - 1] == '/')
    --length;
  if (length == 0)
    return append_char(cookie, '/');
  if (name[0] != '/' && !append_char(cookie, '/'))
    return false;
  for (size_t i = 0; i < length; ++i) {
    unsigned char c = static_cast<unsigned char>(name[i]);
    if (c == ' ' || c == '\t' || c == '\n' || c == '\\') {
      if (!append_char(cookie, '\\') ||
          !append_char(cookie, static_cast<char>('0' + (c >> 6))) ||
          !append_char(cookie, static_cast<char>('0' + ((c >> 3) & 7))) ||
          !append_char(cookie, static_cast<char>('0' + (c & 7))))
        return false;
    } else if (!append_char(cookie, static_cast<char>(c))) {
      return false;
    }
  }
  return true;
}

bool load_mount(MountCookie *cookie) {
  for (int fd = cookie->next_fd;; ++fd) {
    wasi::__wasi_prestat_t prestat;
    if (wasi::__wasi_fd_prestat_get(static_cast<wasi::__wasi_fd_t>(fd),
                                    &prestat) != wasi::__WASI_ERRNO_SUCCESS)
      return false;
    if (prestat.pr_type != wasi::__WASI_PREOPENTYPE_DIR ||
        prestat.pr_name_len >= 4096)
      continue;
    char name[4096];
    if (wasi::__wasi_fd_prestat_dir_name(static_cast<wasi::__wasi_fd_t>(fd),
                                         name, prestat.pr_name_len) !=
        wasi::__WASI_ERRNO_SUCCESS)
      continue;
    cookie->used = 0;
    cookie->position = 0;
    cookie->next_fd = fd + 1;
    if (!append_text(cookie, "wasi:") ||
        !append_number(cookie, static_cast<unsigned>(fd)) ||
        !append_char(cookie, ' ') ||
        !append_mount_path(cookie, name, prestat.pr_name_len) ||
        !append_text(cookie, " wasi "))
      continue;
    wasi::__wasi_fdstat_t rights;
    bool writable =
        wasi::__wasi_fd_fdstat_get(static_cast<wasi::__wasi_fd_t>(fd),
                                   &rights) == wasi::__WASI_ERRNO_SUCCESS &&
        (rights.fs_rights_base & (wasi::__WASI_RIGHT_PATH_CREATE_FILE |
                                  wasi::__WASI_RIGHT_PATH_UNLINK_FILE));
    if (!append_text(cookie, writable ? "rw 0 0\n" : "ro 0 0\n"))
      continue;
    return true;
  }
}

ssize_t mount_read(void *context, char *buffer, size_t size) {
  auto *cookie = static_cast<MountCookie *>(context);
  size_t copied = 0;
  while (copied < size) {
    if (cookie->position == cookie->used && !load_mount(cookie))
      break;
    size_t remaining = cookie->used - cookie->position;
    size_t amount = remaining < size - copied ? remaining : size - copied;
    __builtin_memcpy(buffer + copied, cookie->line + cookie->position, amount);
    copied += amount;
    cookie->position += amount;
  }
  return static_cast<ssize_t>(copied);
}

int mount_close(void *context) {
  ::free(context);
  return 0;
}

bool is_mount_table(const char *path) {
  return path &&
         (!::strcmp(path, "/proc/mounts") || !::strcmp(path, "/etc/mtab"));
}

char *next_field(char *&cursor) {
  while (*cursor == ' ' || *cursor == '\t')
    ++cursor;
  if (*cursor == '\0' || *cursor == '\n' || *cursor == '#')
    return nullptr;
  char *field = cursor;
  while (*cursor && *cursor != ' ' && *cursor != '\t' && *cursor != '\n')
    ++cursor;
  if (*cursor)
    *cursor++ = '\0';
  return field;
}

void decode_field(char *field) {
  char *output = field;
  for (char *input = field; *input; ++input) {
    if (*input == '\\' && input[1] >= '0' && input[1] <= '7' &&
        input[2] >= '0' && input[2] <= '7' && input[3] >= '0' &&
        input[3] <= '7') {
      *output++ = static_cast<char>(((input[1] - '0') << 6) |
                                    ((input[2] - '0') << 3) | (input[3] - '0'));
      input += 3;
    } else {
      *output++ = *input;
    }
  }
  *output = '\0';
}

int parse_number(const char *text) {
  if (!text)
    return 0;
  int value = 0;
  for (; *text >= '0' && *text <= '9'; ++text)
    value = value * 10 + *text - '0';
  return value;
}

bool write_field(::FILE *stream, const char *field) {
  if (!field)
    return false;
  for (const unsigned char *p = reinterpret_cast<const unsigned char *>(field);
       *p; ++p) {
    if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\\') {
      if (::fprintf(stream, "\\%03o", static_cast<unsigned>(*p)) < 0)
        return false;
    } else if (::fputc(*p, stream) == EOF) {
      return false;
    }
  }
  return true;
}
} // namespace

LLVM_LIBC_FUNCTION(::FILE *, setmntent, (const char *path, const char *mode)) {
  if (!path || !mode) {
    libc_errno = EINVAL;
    return nullptr;
  }
  if (!is_mount_table(path))
    return ::fopen(path, mode);
  if (mode[0] != 'r') {
    libc_errno = EROFS;
    return nullptr;
  }
  auto *cookie = static_cast<MountCookie *>(::malloc(sizeof(MountCookie)));
  if (!cookie)
    return nullptr;
  cookie->next_fd = 3;
  cookie->used = 0;
  cookie->position = 0;
  cookie_io_functions_t ops = {mount_read, nullptr, nullptr, mount_close};
  ::FILE *stream = LIBC_NAMESPACE::fopencookie(cookie, "r", ops);
  if (!stream)
    ::free(cookie);
  return stream;
}

LLVM_LIBC_FUNCTION(struct mntent *, getmntent_r,
                   (::FILE * stream, struct mntent *result, char *buffer,
                    int size)) {
  if (!stream || !result || !buffer || size < 2) {
    libc_errno = EINVAL;
    return nullptr;
  }
  while (::fgets(buffer, size, stream)) {
    size_t length = ::strlen(buffer);
    if (length == static_cast<size_t>(size - 1) && buffer[length - 1] != '\n' &&
        !::feof(stream)) {
      char discard[64];
      do {
        if (!::fgets(discard, sizeof(discard), stream))
          break;
      } while (!::strchr(discard, '\n'));
      libc_errno = ERANGE;
      return nullptr;
    }
    char *cursor = buffer;
    char *source = next_field(cursor);
    char *directory = next_field(cursor);
    char *type = next_field(cursor);
    char *options = next_field(cursor);
    if (!source || !directory || !type || !options)
      continue;
    decode_field(source);
    decode_field(directory);
    decode_field(type);
    decode_field(options);
    result->mnt_fsname = source;
    result->mnt_dir = directory;
    result->mnt_type = type;
    result->mnt_opts = options;
    result->mnt_freq = parse_number(next_field(cursor));
    result->mnt_passno = parse_number(next_field(cursor));
    return result;
  }
  return nullptr;
}

LLVM_LIBC_FUNCTION(struct mntent *, getmntent, (::FILE * stream)) {
  static struct mntent result;
  static char buffer[MAX_MOUNT_LINE];
  return LIBC_NAMESPACE::getmntent_r(stream, &result, buffer, sizeof(buffer));
}

LLVM_LIBC_FUNCTION(int, endmntent, (::FILE * stream)) {
  if (!stream) {
    libc_errno = EINVAL;
    return 0;
  }
  return ::fclose(stream) == 0;
}

LLVM_LIBC_FUNCTION(int, addmntent,
                   (::FILE * stream, const struct mntent *entry)) {
  if (!stream || !entry) {
    libc_errno = EINVAL;
    return 1;
  }
  const char *fields[] = {entry->mnt_fsname, entry->mnt_dir, entry->mnt_type,
                          entry->mnt_opts};
  for (const char *field : fields)
    if (!write_field(stream, field) || ::fputc(' ', stream) == EOF)
      return 1;
  return ::fprintf(stream, "%d %d\n", entry->mnt_freq, entry->mnt_passno) < 0;
}

LLVM_LIBC_FUNCTION(char *, hasmntopt,
                   (const struct mntent *entry, const char *option)) {
  if (!entry || !entry->mnt_opts || !option)
    return nullptr;
  size_t length = ::strlen(option);
  char *cursor = entry->mnt_opts;
  while (*cursor) {
    if (!::strncmp(cursor, option, length) &&
        (cursor[length] == '\0' || cursor[length] == ',' ||
         cursor[length] == '='))
      return cursor;
    while (*cursor && *cursor != ',')
      ++cursor;
    if (*cursor)
      ++cursor;
  }
  return nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
