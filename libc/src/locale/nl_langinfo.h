#ifndef LLVM_LIBC_SRC_LOCALE_NL_LANGINFO_H
#define LLVM_LIBC_SRC_LOCALE_NL_LANGINFO_H
#include "src/__support/macros/config.h"
#include <langinfo.h>
namespace LIBC_NAMESPACE_DECL {
char *nl_langinfo(nl_item);
}
#endif
