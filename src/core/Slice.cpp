#include "Slice.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

using enum Slice;

std::string toStr(const Slice& slice) {
  switch (slice) {
    case M:
      return "M";
    case E:
      return "E";
    case S:
      return "S";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<size_t, Slice> parseSlice(const std::string& str) {
  if (str.empty()) return {0, {}};
  switch (str[0]) {
    case 'M':
      return {1, M};
    case 'E':
      return {1, E};
    case 'S':
      return {1, S};
    default:
      return {0, {}};
  }
}
