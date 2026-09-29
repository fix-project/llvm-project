#ifndef LLVM_LIBC_SRC_STDIO_TMPFILE_H
#define LLVM_LIBC_SRC_STDIO_TMPFILE_H
#include "hdr/types/FILE.h"
#include "src/__support/macros/config.h"
namespace LIBC_NAMESPACE_DECL {
::FILE *tmpfile();
}
#endif
