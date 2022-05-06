#include "Slice.h"
#include <stdexcept>

Face getRotationFace(const Slice& slice) {
  switch (slice) {
    case Slice::M:
      return Face::L;
    case Slice::E:
      return Face::D;
    case Slice::S:
      return Face::F;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<RotationAxis, bool> getRotationAxis(const Slice& slice) {
  switch (slice) {
    case Slice::M:
      return {RotationAxis::X, true};
    case Slice::E:
      return {RotationAxis::Y, true};
    case Slice::S:
      return {RotationAxis::Z, false};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<Slice, bool> fromRotationFace(const Face& face) {
  switch (face) {
    case Face::L:
      return {Slice::M, false};
    case Face::R:
      return {Slice::M, true};
    case Face::D:
      return {Slice::E, false};
    case Face::U:
      return {Slice::E, true};
    case Face::F:
      return {Slice::S, false};
    case Face::B:
      return {Slice::S, true};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStr(const Slice& face) {
  switch (face) {
    case Slice::M:
      return "M";
    case Slice::E:
      return "E";
    case Slice::S:
      return "S";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<int, Slice> parseSlice(const std::string& str) {
  if (str.empty()) return {0, {}};
  char first_char = str[0];
  if (first_char == 'M')
    return {1, Slice::M};
  else if (first_char == 'E')
    return {1, Slice::E};
  else if (first_char == 'S')
    return {1, Slice::S};
  else
    return {0, {}};
}
