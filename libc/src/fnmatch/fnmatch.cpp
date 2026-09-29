//===-- Portable fnmatch implementation ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/fnmatch/fnmatch.h"

#include "hdr/fnmatch_macros.h"
#include "src/__support/common.h"

namespace LIBC_NAMESPACE_DECL {
namespace {

unsigned char fold(unsigned char c, int flags) {
  if ((flags & FNM_CASEFOLD) && c >= 'A' && c <= 'Z')
    return c + ('a' - 'A');
  return c;
}

bool equal(unsigned char a, unsigned char b, int flags) {
  return fold(a, flags) == fold(b, flags);
}

bool class_name(const char *begin, const char *end, const char *name) {
  while (begin != end && *name != '\0' && *begin == *name) {
    ++begin;
    ++name;
  }
  return begin == end && *name == '\0';
}

bool character_class(const char *begin, const char *end, unsigned char c) {
  bool lower = c >= 'a' && c <= 'z';
  bool upper = c >= 'A' && c <= 'Z';
  bool digit = c >= '0' && c <= '9';
  bool space = c == ' ' || (c >= '\t' && c <= '\r');
  if (class_name(begin, end, "alnum"))
    return lower || upper || digit;
  if (class_name(begin, end, "alpha"))
    return lower || upper;
  if (class_name(begin, end, "blank"))
    return c == ' ' || c == '\t';
  if (class_name(begin, end, "cntrl"))
    return c < 0x20 || c == 0x7f;
  if (class_name(begin, end, "digit"))
    return digit;
  if (class_name(begin, end, "graph"))
    return c > 0x20 && c < 0x7f;
  if (class_name(begin, end, "lower"))
    return lower;
  if (class_name(begin, end, "print"))
    return c >= 0x20 && c < 0x7f;
  if (class_name(begin, end, "punct"))
    return c > 0x20 && c < 0x7f && !(lower || upper || digit);
  if (class_name(begin, end, "space"))
    return space;
  if (class_name(begin, end, "upper"))
    return upper;
  if (class_name(begin, end, "xdigit"))
    return digit || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
  return false;
}

// Match a bracket expression and return the position after its closing ']'.
// A missing closing bracket makes '[' an ordinary character.
bool bracket(const char *pattern, unsigned char c, int flags, const char *&next,
             bool &matched) {
  const char *p = pattern + 1;
  bool inverse = *p == '!' || *p == '^';
  if (inverse)
    ++p;
  bool first = true;
  bool found = false;
  for (; *p != '\0';) {
    if (*p == ']' && !first) {
      next = p + 1;
      matched = inverse ? !found : found;
      return true;
    }
    first = false;
    if (p[0] == '[' && p[1] == ':') {
      const char *name = p + 2;
      const char *end = name;
      while (*end != '\0' && !(end[0] == ':' && end[1] == ']'))
        ++end;
      if (*end != '\0') {
        found |= character_class(name, end, c);
        p = end + 2;
        continue;
      }
    }
    unsigned char start = static_cast<unsigned char>(*p++);
    if (start == '\\' && !(flags & FNM_NOESCAPE) && *p != '\0')
      start = static_cast<unsigned char>(*p++);
    if (*p == '-' && p[1] != '\0' && p[1] != ']') {
      ++p;
      unsigned char end = static_cast<unsigned char>(*p++);
      if (end == '\\' && !(flags & FNM_NOESCAPE) && *p != '\0')
        end = static_cast<unsigned char>(*p++);
      unsigned char folded = fold(c, flags);
      found |= fold(start, flags) <= folded && folded <= fold(end, flags);
    } else {
      found |= equal(start, c, flags);
    }
  }
  return false;
}

bool wildcard_allowed(const char *begin, const char *s, int flags) {
  if (*s == '\0')
    return false;
  if ((flags & FNM_PATHNAME) && *s == '/')
    return false;
  if ((flags & FNM_PERIOD) && *s == '.' &&
      (s == begin || ((flags & FNM_PATHNAME) && s[-1] == '/')))
    return false;
  return true;
}

} // namespace

LLVM_LIBC_FUNCTION(int, fnmatch,
                   (const char *pattern, const char *string, int flags)) {
  const char *begin = string;
  const char *star_pattern = nullptr;
  const char *star_string = nullptr;

  for (;;) {
    if (*pattern == '*') {
      while (*pattern == '*')
        ++pattern;
      star_pattern = pattern;
      star_string = string;
      if (*pattern == '\0') {
        if (!(flags & FNM_PATHNAME)) {
          if (!(flags & FNM_PERIOD) || !(string == begin && *string == '.'))
            return 0;
        } else {
          while (wildcard_allowed(begin, string, flags))
            ++string;
          if (*string == '\0' || ((flags & FNM_LEADING_DIR) && *string == '/'))
            return 0;
        }
      }
    }

    if (*pattern == '\0') {
      if (*string == '\0' || ((flags & FNM_LEADING_DIR) && *string == '/'))
        return 0;
    } else if (*string != '\0') {
      if (*pattern == '/' && *string == '/') {
        ++pattern;
        ++string;
        continue;
      }
      if (wildcard_allowed(begin, string, flags)) {
        if (*pattern == '?') {
          ++pattern;
          ++string;
          continue;
        }
        if (*pattern == '[') {
          const char *next;
          bool matched;
          if (bracket(pattern, static_cast<unsigned char>(*string), flags, next,
                      matched)) {
            if (matched) {
              pattern = next;
              ++string;
              continue;
            }
          } else if (*string == '[') {
            ++pattern;
            ++string;
            continue;
          }
        } else {
          const char *next = pattern + 1;
          unsigned char literal = static_cast<unsigned char>(*pattern);
          if (literal == '\\' && !(flags & FNM_NOESCAPE) && *next != '\0')
            literal = static_cast<unsigned char>(*next++);
          if (equal(literal, static_cast<unsigned char>(*string), flags)) {
            pattern = next;
            ++string;
            continue;
          }
        }
      } else if (*pattern != '?' && *pattern != '[' &&
                 equal(static_cast<unsigned char>(*pattern),
                       static_cast<unsigned char>(*string), flags)) {
        ++pattern;
        ++string;
        continue;
      }
    }

    if (star_pattern != nullptr &&
        wildcard_allowed(begin, star_string, flags)) {
      string = ++star_string;
      pattern = star_pattern;
      continue;
    }
    return FNM_NOMATCH;
  }
}

} // namespace LIBC_NAMESPACE_DECL
