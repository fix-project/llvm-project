//===-- gethostbyaddr entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_GETHOSTBYADDR_H
#define LLVM_LIBC_SRC_NETDB_GETHOSTBYADDR_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_GETHOSTBYADDR_H
