#include "src/iconv/iconv.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

namespace LIBC_NAMESPACE_DECL {
namespace {
enum class Encoding { UTF8, ASCII, LATIN1, UTF16, UTF16LE, UTF16BE, INVALID };
struct Converter {
  Encoding from;
  Encoding to;
  bool input_started;
  bool input_little_endian;
  bool output_started;
};

Encoding parse_encoding(const char *name) {
  if (!name)
    return Encoding::INVALID;
  char normalized[24];
  size_t length = 0;
  for (; *name; ++name) {
    unsigned char c = static_cast<unsigned char>(*name);
    if (c == '-' || c == '_' || c == ' ')
      continue;
    if (length + 1 >= sizeof(normalized))
      return Encoding::INVALID;
    normalized[length++] = c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
  }
  normalized[length] = '\0';
  auto equals = [&](const char *other) {
    size_t i = 0;
    for (; normalized[i] && other[i]; ++i)
      if (normalized[i] != other[i])
        return false;
    return normalized[i] == other[i];
  };
  if (equals("UTF8"))
    return Encoding::UTF8;
  if (equals("ASCII") || equals("USASCII"))
    return Encoding::ASCII;
  if (equals("ISO88591") || equals("LATIN1"))
    return Encoding::LATIN1;
  if (equals("UTF16"))
    return Encoding::UTF16;
  if (equals("UTF16LE"))
    return Encoding::UTF16LE;
  if (equals("UTF16BE"))
    return Encoding::UTF16BE;
  return Encoding::INVALID;
}

uint16_t read16(const unsigned char *p, bool little) {
  return little ? static_cast<uint16_t>(p[0] | p[1] << 8)
                : static_cast<uint16_t>(p[0] << 8 | p[1]);
}

void write16(unsigned char *p, uint16_t value, bool little) {
  p[0] = static_cast<unsigned char>(little ? value : value >> 8);
  p[1] = static_cast<unsigned char>(little ? value >> 8 : value);
}

int decode(const unsigned char *p, size_t available, Encoding encoding,
           bool little, uint32_t &codepoint, size_t &used) {
  if (!available)
    return EINVAL;
  if (encoding == Encoding::ASCII || encoding == Encoding::LATIN1) {
    codepoint = p[0];
    used = 1;
    return encoding == Encoding::ASCII && codepoint > 0x7f ? EILSEQ : 0;
  }
  if (encoding == Encoding::UTF8) {
    unsigned char first = p[0];
    used = first < 0x80                     ? 1
           : first >= 0xc2 && first <= 0xdf ? 2
           : first >= 0xe0 && first <= 0xef ? 3
           : first >= 0xf0 && first <= 0xf4 ? 4
                                            : 0;
    if (!used)
      return EILSEQ;
    if (available < used)
      return EINVAL;
    codepoint = first & (used == 1 ? 0x7f : (0x7f >> used));
    for (size_t i = 1; i < used; ++i) {
      if ((p[i] & 0xc0) != 0x80)
        return EILSEQ;
      codepoint = (codepoint << 6) | (p[i] & 0x3f);
    }
    if ((used == 2 && codepoint < 0x80) || (used == 3 && codepoint < 0x800) ||
        (used == 4 && codepoint < 0x10000) ||
        (codepoint >= 0xd800 && codepoint <= 0xdfff) || codepoint > 0x10ffff)
      return EILSEQ;
    return 0;
  }
  if (available < 2)
    return EINVAL;
  uint16_t first = read16(p, little);
  used = 2;
  if (first >= 0xdc00 && first <= 0xdfff)
    return EILSEQ;
  if (first >= 0xd800 && first <= 0xdbff) {
    if (available < 4)
      return EINVAL;
    uint16_t second = read16(p + 2, little);
    if (second < 0xdc00 || second > 0xdfff)
      return EILSEQ;
    codepoint = 0x10000 + ((first - 0xd800) << 10) + second - 0xdc00;
    used = 4;
  } else {
    codepoint = first;
  }
  return 0;
}

int encode(uint32_t cp, Encoding encoding, unsigned char *out, size_t &size) {
  if (encoding == Encoding::ASCII || encoding == Encoding::LATIN1) {
    if (cp > (encoding == Encoding::ASCII ? 0x7fU : 0xffU))
      return EILSEQ;
    out[0] = static_cast<unsigned char>(cp);
    size = 1;
  } else if (encoding == Encoding::UTF8) {
    size = cp < 0x80 ? 1 : cp < 0x800 ? 2 : cp < 0x10000 ? 3 : 4;
    if (size == 1) {
      out[0] = static_cast<unsigned char>(cp);
    } else {
      for (size_t i = size - 1; i > 0; --i) {
        out[i] = static_cast<unsigned char>(0x80 | (cp & 0x3f));
        cp >>= 6;
      }
      out[0] = static_cast<unsigned char>((0xffU << (8 - size)) | cp);
    }
  } else {
    bool little = encoding == Encoding::UTF16LE;
    if (cp < 0x10000) {
      write16(out, static_cast<uint16_t>(cp), little);
      size = 2;
    } else {
      cp -= 0x10000;
      write16(out, static_cast<uint16_t>(0xd800 | (cp >> 10)), little);
      write16(out + 2, static_cast<uint16_t>(0xdc00 | (cp & 0x3ff)), little);
      size = 4;
    }
  }
  return 0;
}
} // namespace

LLVM_LIBC_FUNCTION(iconv_t, iconv_open,
                   (const char *to_name, const char *from_name)) {
  Encoding to = parse_encoding(to_name);
  Encoding from = parse_encoding(from_name);
  if (to == Encoding::INVALID || from == Encoding::INVALID) {
    libc_errno = EINVAL;
    return reinterpret_cast<iconv_t>(-1);
  }
  auto *converter = static_cast<Converter *>(::malloc(sizeof(Converter)));
  if (!converter) {
    libc_errno = ENOMEM;
    return reinterpret_cast<iconv_t>(-1);
  }
  *converter = {from, to, false, false, false};
  return converter;
}

LLVM_LIBC_FUNCTION(size_t, iconv,
                   (iconv_t handle, char **input, size_t *input_left,
                    char **output, size_t *output_left)) {
  if (!handle || handle == reinterpret_cast<iconv_t>(-1)) {
    libc_errno = EBADF;
    return static_cast<size_t>(-1);
  }
  auto &converter = *static_cast<Converter *>(handle);
  if (!input || !*input) {
    converter.input_started = false;
    converter.output_started = false;
    return 0;
  }
  if (!input_left || !output || !*output || !output_left) {
    libc_errno = EINVAL;
    return static_cast<size_t>(-1);
  }
  auto *in = reinterpret_cast<unsigned char *>(*input);
  auto *out = reinterpret_cast<unsigned char *>(*output);
  size_t in_left = *input_left;
  size_t out_left = *output_left;
  int error = 0;
  while (in_left) {
    if (converter.from == Encoding::UTF16 && !converter.input_started) {
      if (in_left < 2) {
        error = EINVAL;
        break;
      }
      converter.input_little_endian = in[0] == 0xff && in[1] == 0xfe;
      converter.input_started = true;
      if (converter.input_little_endian || (in[0] == 0xfe && in[1] == 0xff)) {
        in += 2;
        in_left -= 2;
        continue;
      }
    }
    uint32_t cp;
    size_t used;
    Encoding source = converter.from;
    bool little = source == Encoding::UTF16LE ||
                  (source == Encoding::UTF16 && converter.input_little_endian);
    error = decode(in, in_left, source, little, cp, used);
    if (error)
      break;
    unsigned char encoded[6];
    size_t encoded_size;
    Encoding destination =
        converter.to == Encoding::UTF16 ? Encoding::UTF16BE : converter.to;
    error = encode(cp, destination, encoded + 2, encoded_size);
    if (error)
      break;
    size_t bom_size =
        converter.to == Encoding::UTF16 && !converter.output_started ? 2 : 0;
    if (out_left < encoded_size + bom_size) {
      error = E2BIG;
      break;
    }
    if (bom_size) {
      out[0] = 0xfe;
      out[1] = 0xff;
      out += 2;
      out_left -= 2;
    }
    for (size_t i = 0; i < encoded_size; ++i)
      out[i] = encoded[i + 2];
    out += encoded_size;
    out_left -= encoded_size;
    in += used;
    in_left -= used;
    converter.output_started = true;
  }
  *input = reinterpret_cast<char *>(in);
  *output = reinterpret_cast<char *>(out);
  *input_left = in_left;
  *output_left = out_left;
  if (error) {
    libc_errno = error;
    return static_cast<size_t>(-1);
  }
  return 0;
}

LLVM_LIBC_FUNCTION(int, iconv_close, (iconv_t handle)) {
  if (!handle || handle == reinterpret_cast<iconv_t>(-1)) {
    libc_errno = EBADF;
    return -1;
  }
  ::free(handle);
  return 0;
}
} // namespace LIBC_NAMESPACE_DECL
