#ifndef LLVM_LIBC_SRC_ARPA_INET_INET_NTOA_H
#define LLVM_LIBC_SRC_ARPA_INET_INET_NTOA_H
#include "hdr/types/struct_in_addr.h"
#include "src/__support/macros/config.h"
namespace LIBC_NAMESPACE_DECL {
char *inet_ntoa(struct in_addr);
}
#endif
