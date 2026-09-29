//===-- setservent entrypoint
//-----------------------------------------------===//

#ifndef LLVM_LIBC_SRC_NETDB_SETSERVENT_H
#define LLVM_LIBC_SRC_NETDB_SETSERVENT_H
#include "src/__support/macros/config.h"
#include <netdb.h>
namespace LIBC_NAMESPACE_DECL {
void setservent(int stayopen);
} // namespace LIBC_NAMESPACE_DECL
#endif // LLVM_LIBC_SRC_NETDB_SETSERVENT_H
