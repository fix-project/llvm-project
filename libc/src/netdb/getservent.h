//===-- getservent entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_GETSERVENT_H
#define LLVM_LIBC_SRC_NETDB_GETSERVENT_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
struct servent *getservent();
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_GETSERVENT_H
