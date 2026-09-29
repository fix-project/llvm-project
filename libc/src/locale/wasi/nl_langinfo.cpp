#include "src/locale/nl_langinfo.h"
#include "src/__support/common.h"

namespace LIBC_NAMESPACE_DECL {
LLVM_LIBC_FUNCTION(char *, nl_langinfo, (nl_item item)) {
  static const char *const days[] = {"Sunday",    "Monday",   "Tuesday",
                                     "Wednesday", "Thursday", "Friday",
                                     "Saturday"};
  static const char *const short_days[] = {"Sun", "Mon", "Tue", "Wed",
                                           "Thu", "Fri", "Sat"};
  static const char *const months[] = {
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};
  static const char *const short_months[] = {"Jan", "Feb", "Mar", "Apr",
                                             "May", "Jun", "Jul", "Aug",
                                             "Sep", "Oct", "Nov", "Dec"};

  if (item >= DAY_1 && item <= DAY_7)
    return const_cast<char *>(days[item - DAY_1]);
  if (item >= ABDAY_1 && item <= ABDAY_7)
    return const_cast<char *>(short_days[item - ABDAY_1]);
  if (item >= MON_1 && item <= MON_12)
    return const_cast<char *>(months[item - MON_1]);
  if (item >= ABMON_1 && item <= ABMON_12)
    return const_cast<char *>(short_months[item - ABMON_1]);

  switch (item) {
  case CODESET:
    return const_cast<char *>("US-ASCII");
  case RADIXCHAR:
    return const_cast<char *>(".");
  case THOUSEP:
    return const_cast<char *>("");
  case CRNCYSTR:
    return const_cast<char *>("");
  case AM_STR:
    return const_cast<char *>("AM");
  case PM_STR:
    return const_cast<char *>("PM");
  case D_T_FMT:
    return const_cast<char *>("%a %b %e %H:%M:%S %Y");
  case D_FMT:
    return const_cast<char *>("%m/%d/%y");
  case T_FMT:
    return const_cast<char *>("%H:%M:%S");
  default:
    return const_cast<char *>("");
  }
}
} // namespace LIBC_NAMESPACE_DECL
