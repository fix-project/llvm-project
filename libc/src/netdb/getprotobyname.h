//===-- getprotobyname entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_GETPROTOBYNAME_H
#define LLVM_LIBC_SRC_NETDB_GETPROTOBYNAME_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
struct protoent *getprotobyname(const char *name);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_GETPROTOBYNAME_H
