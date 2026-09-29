#include "src/stdio/tmpfile.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/stdio/generic/temp_path.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(::FILE *, tmpfile, ()) {
  char path[FILENAME_MAX];
  if (!make_temp_template(path, sizeof(path)))
    return nullptr;
  int fd = mkstemp(path);
  if (fd < 0)
    return nullptr;
  if (unlink(path) != 0) {
    int saved = libc_errno;
    close(fd);
    libc_errno = saved;
    return nullptr;
  }
  ::FILE *stream = fdopen(fd, "w+b");
  if (!stream) {
    int saved = libc_errno;
    close(fd);
    libc_errno = saved;
  }
  return stream;
}
} // namespace LIBC_NAMESPACE_DECL
