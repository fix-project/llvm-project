//===-- Portable long option parsing --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getopt_long.h"
#include "src/unistd/getopt.h"
#include "src/__support/common.h"
#include "src/stdio/fprintf.h"
#include "src/stdio/stderr.h"
#include "src/stdlib/getenv.h"

namespace LIBC_NAMESPACE_DECL {
namespace {

// Permutation state is shared by getopt_long and getopt_long_only. The short
// option position comes from getopt so callers can switch parsers mid-cluster.
int option_start = 1;
int nonoption_start = 1;
int previous_optind = 1;
bool ended = false;
char *const *previous_argv = nullptr;

bool is_option(const char *arg) {
  return arg != nullptr && arg[0] == '-' && arg[1] != '\0';
}

bool short_option_exists(const char *optstring, char option_char) {
  for (const char *p = optstring; *p != '\0'; ++p) {
    if (*p == option_char)
      return true;
  }
  return false;
}

struct LongMatch {
  int index = -1;
  bool ambiguous = false;
  bool exact = false;
  const char *end;
};

LongMatch find_long_option(const char *name, const struct option *longopts,
                           bool long_only) {
  LongMatch found;
  found.end = name;
  while (*found.end != '\0' && *found.end != '=')
    ++found.end;
  int name_length = static_cast<int>(found.end - name);
  if (name_length == 0)
    return found;

  for (int i = 0; longopts != nullptr && longopts[i].name != nullptr; ++i) {
    const char *candidate = longopts[i].name;
    int j = 0;
    while (j < name_length && candidate[j] != '\0' && candidate[j] == name[j])
      ++j;
    if (j != name_length)
      continue;
    if (candidate[j] == '\0') {
      found.index = i;
      found.exact = true;
      found.ambiguous = false;
      break;
    }
    if (found.index < 0) {
      found.index = i;
    } else {
      const struct option &previous = longopts[found.index];
      const struct option &current = longopts[i];
      if (long_only || previous.has_arg != current.has_arg ||
          previous.flag != current.flag || previous.val != current.val)
        found.ambiguous = true;
    }
  }
  return found;
}

// Move a parsed option (and its separate argument, if present) in front of
// intervening operands. Reversal avoids allocating a temporary argv array.
void reverse(char *const argv[], int first, int last) {
  char **mutable_argv = const_cast<char **>(argv);
  while (first < --last) {
    char *tmp = mutable_argv[first];
    mutable_argv[first++] = mutable_argv[last];
    mutable_argv[last] = tmp;
  }
}

void permute(char *const argv[], int first, int middle, int last) {
  if (first >= middle || middle >= last)
    return;
  reverse(argv, first, middle);
  reverse(argv, middle, last);
  reverse(argv, first, last);
  optind = first + last - middle;
}

int parse(int argc, char *const argv[], const char *optstring,
          const struct option *longopts, int *longindex, bool long_only) {
  unsigned &short_position = impl::getopt_short_position();
  if (optind == 0 ||
      (optind == 1 && !short_position && (previous_optind != 1 || ended))) {
    optind = 1;
    short_position = 0;
    nonoption_start = 1;
    ended = false;
  } else if (argv != previous_argv) {
    // getopt may already be in the middle of a short-option cluster.
    nonoption_start = optind;
    option_start = optind;
    ended = false;
  }
  previous_argv = argv;
  previous_optind = optind;
  if (ended)
    return -1;

  optarg = nullptr;
  bool return_operands = optstring[0] == '-';
  bool stop_at_operand = optstring[0] == '+' ||
                         (!return_operands && getenv("POSIXLY_CORRECT"));
  bool missing_colon = optstring[0] == ':' ||
                       ((optstring[0] == '+' || optstring[0] == '-') &&
                        optstring[1] == ':');

  if (!short_position) {
    nonoption_start = optind;
    while (optind < argc && argv[optind] != nullptr &&
           !is_option(argv[optind])) {
      if (stop_at_operand) {
        ended = true;
        return -1;
      }
      if (return_operands) {
        optarg = argv[optind++];
        previous_optind = optind;
        return 1;
      }
      ++optind;
    }

    if (optind >= argc || argv[optind] == nullptr) {
      ended = true;
      optind = nonoption_start;
      previous_optind = optind;
      return -1;
    }

    option_start = optind;
    if (argv[optind][0] == '-' && argv[optind][1] == '-' &&
        argv[optind][2] == '\0') {
      ++optind;
      permute(argv, nonoption_start, option_start, optind);
      ended = true;
      previous_optind = optind;
      return -1;
    }
  }

  const char *arg = argv[optind];
  const bool double_dash = arg[1] == '-';
  const char *long_name = arg + (double_dash ? 2 : 1);
  bool try_long = !short_position &&
                  (double_dash ||
                   (longopts != nullptr && long_only &&
                    (arg[2] != '\0' ||
                     !short_option_exists(optstring, arg[1]))));

  auto finish_long = [&](const LongMatch &found, const char *display) {
    int result = '?';
    bool accepted = false;
    if (found.index < 0 || found.ambiguous) {
      optopt = 0;
      if (opterr && !missing_colon)
        fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], display);
    } else {
      const struct option &chosen = longopts[found.index];
      if (longindex != nullptr)
        *longindex = found.index;
      if (*found.end == '=' && chosen.has_arg == no_argument) {
        optopt = chosen.val;
        if (opterr && !missing_colon)
          fprintf(stderr, "%s: option '%s' does not allow an argument\n",
                  argv[0], display);
      } else if (*found.end == '=') {
        optarg = const_cast<char *>(found.end + 1);
        result = chosen.val;
        accepted = true;
      } else if (chosen.has_arg == required_argument) {
        if (optind < argc && argv[optind] != nullptr) {
          optarg = argv[optind++];
          result = chosen.val;
          accepted = true;
        } else {
          optopt = chosen.val;
          result = missing_colon ? ':' : '?';
          if (opterr && !missing_colon)
            fprintf(stderr, "%s: option '%s' requires an argument\n",
                    argv[0], display);
        }
      } else {
        result = chosen.val;
        accepted = true;
      }
      if (accepted && chosen.flag != nullptr) {
        *chosen.flag = chosen.val;
        result = 0;
      }
    }
    permute(argv, nonoption_start, option_start, optind);
    previous_optind = optind;
    return result;
  };

  if (try_long) {
    LongMatch found = find_long_option(long_name, longopts, long_only);
    // getopt_long_only falls back to short options if no long name matches.
    if (found.exact || found.index >= 0 || double_dash ||
        !short_option_exists(optstring, arg[1])) {
      ++optind;
      return finish_long(found, arg);
    }
  }

  if (!short_position)
    short_position = 1;
  char ch = arg[short_position++];
  const char *spec = optstring;
  while (*spec == '+' || *spec == '-' || *spec == ':')
    ++spec;
  while (*spec != '\0' && *spec != ch)
    ++spec;

  int result = ch;
  if (*spec == '\0' || ch == ':' || ch == ';') {
    optopt = ch;
    result = '?';
    if (opterr && !missing_colon)
      fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], ch);
  } else if (ch == 'W' && spec[1] == ';' && longopts != nullptr) {
    // GNU's W; extension interprets the short option's argument as a long
    // option name. Both -Wname and -W name use the ordinary long-option path.
    const char *name = nullptr;
    if (arg[short_position] != '\0') {
      name = arg + short_position;
      ++optind;
    } else if (optind + 1 < argc && argv[optind + 1] != nullptr) {
      name = argv[optind + 1];
      optind += 2;
    } else {
      optopt = 'W';
      result = missing_colon ? ':' : '?';
      if (opterr && !missing_colon)
        fprintf(stderr, "%s: option requires an argument -- 'W'\n", argv[0]);
      ++optind;
    }
    short_position = 0;
    if (name != nullptr)
      return finish_long(find_long_option(name, longopts, false), name);
  } else if (spec[1] == ':') {
    if (arg[short_position] != '\0') {
      optarg = const_cast<char *>(arg + short_position);
    } else if (spec[2] != ':' && optind + 1 < argc &&
               argv[optind + 1] != nullptr) {
      optarg = argv[++optind];
    } else if (spec[2] != ':') {
      optopt = ch;
      result = missing_colon ? ':' : '?';
      if (opterr && !missing_colon)
        fprintf(stderr, "%s: option requires an argument -- '%c'\n", argv[0],
                ch);
    }
    short_position = 0;
    ++optind;
  }

  if (short_position && arg[short_position] == '\0') {
    short_position = 0;
    ++optind;
  }
  if (!short_position)
    permute(argv, nonoption_start, option_start, optind);
  previous_optind = optind;
  return result;
}
} // namespace

LLVM_LIBC_FUNCTION(int, getopt_long,
                   (int argc, char *const argv[], const char *optstring,
                    const struct option *longopts, int *longindex)) {
  return parse(argc, argv, optstring, longopts, longindex, false);
}

LLVM_LIBC_FUNCTION(int, getopt_long_only,
                   (int argc, char *const argv[], const char *optstring,
                    const struct option *longopts, int *longindex)) {
  return parse(argc, argv, optstring, longopts, longindex, true);
}

} // namespace LIBC_NAMESPACE_DECL
