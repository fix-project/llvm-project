#ifndef LLVM_LIBC_LANGINFO_H
#define LLVM_LIBC_LANGINFO_H

#include "__llvm-libc-common.h"

typedef int nl_item;
#define CODESET 1
#define RADIXCHAR 2
#define THOUSEP 3
#define AM_STR 4
#define PM_STR 5
#define D_T_FMT 6
#define D_FMT 7
#define T_FMT 8
#define DAY_1 9
#define DAY_2 10
#define DAY_3 11
#define DAY_4 12
#define DAY_5 13
#define DAY_6 14
#define DAY_7 15
#define ABDAY_1 16
#define ABDAY_2 17
#define ABDAY_3 18
#define ABDAY_4 19
#define ABDAY_5 20
#define ABDAY_6 21
#define ABDAY_7 22
#define MON_1 23
#define MON_2 24
#define MON_3 25
#define MON_4 26
#define MON_5 27
#define MON_6 28
#define MON_7 29
#define MON_8 30
#define MON_9 31
#define MON_10 32
#define MON_11 33
#define MON_12 34
#define ABMON_1 35
#define ABMON_2 36
#define ABMON_3 37
#define ABMON_4 38
#define ABMON_5 39
#define ABMON_6 40
#define ABMON_7 41
#define ABMON_8 42
#define ABMON_9 43
#define ABMON_10 44
#define ABMON_11 45
#define ABMON_12 46
#define CRNCYSTR 47

__BEGIN_C_DECLS
char *nl_langinfo(nl_item) __NOEXCEPT;
__END_C_DECLS
#endif
