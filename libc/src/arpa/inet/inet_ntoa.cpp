#include "src/arpa/inet/inet_ntoa.h"
#include "src/__support/CPP/span.h"
#include "src/__support/common.h"
#include "src/__support/net/address.h"

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(char *, inet_ntoa, (struct in_addr addr)) {
  static char buffer[16];
  return net::ipv4_to_str(addr, cpp::span<char>(buffer, sizeof(buffer)))
             ? buffer
             : nullptr;
}
} // namespace LIBC_NAMESPACE_DECL
