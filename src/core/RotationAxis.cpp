#include "RotationAxis.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

std::string toStr(const RotationAxis& rotationAxis) {
  switch (rotationAxis) {
    case RotationAxis::X:
      return "X";
    case RotationAxis::Y:
      return "Y";
    case RotationAxis::Z:
      return "Z";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<size_t, RotationAxis> parseRotationAxis(const std::string& str) {
  if (str.empty()) return {0, {}};
  const char& first_char = str[0];
  if (first_char == 'X' || first_char == 'x')
    return {1, RotationAxis::X};
  else if (first_char == 'Y' || first_char == 'y')
    return {1, RotationAxis::Y};
  else if (first_char == 'Z' || first_char == 'z')
    return {1, RotationAxis::Z};
  else
    return {0, {}};
}
