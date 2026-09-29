//===-- getservbyport entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_GETSERVBYPORT_H
#define LLVM_LIBC_SRC_NETDB_GETSERVBYPORT_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
struct servent *getservbyport(int port, const char *proto);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_GETSERVBYPORT_H
