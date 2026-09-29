//===-- endservent entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_ENDSERVENT_H
#define LLVM_LIBC_SRC_NETDB_ENDSERVENT_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
void endservent();
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_ENDSERVENT_H
