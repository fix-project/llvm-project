//===-- getservbyname entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_GETSERVBYNAME_H
#define LLVM_LIBC_SRC_NETDB_GETSERVBYNAME_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
struct servent *getservbyname(const char *name, const char *proto);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_GETSERVBYNAME_H
