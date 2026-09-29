//===-- gethostbyname entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_GETHOSTBYNAME_H
#define LLVM_LIBC_SRC_NETDB_GETHOSTBYNAME_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
struct hostent *gethostbyname(const char *name);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_GETHOSTBYNAME_H
