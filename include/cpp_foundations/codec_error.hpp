#pragma once

#include <cstdint>
#include <string_view>

namespace cf {

enum class CodecError : std::uint8_t {
  kBufferTooShort,  // read/write past the end
  kInvalidWidth,    // field width outside 1-64 bits
};

// Helper to convert error to string
constexpr std::string_view ToString(CodecError err) {
  switch (err) {
    case CodecError::kBufferTooShort:
      return "Read/Write past the end of buffer";
    case CodecError::kInvalidWidth:
      return "Invalid field width";
  }
  return "Unknown Error";
}

}  // namespace cf
