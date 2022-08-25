#include "Face.h"
#include "CornerLocation.h"
#include "Cube.h"
#include "RotationAxis.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

using enum Face;

Face getLeft(const Face& top, const Face& front) {
  for (const CornerLocation& corner : Cube::CornerLocationOrder) {
    if (top == corner.first && front == corner.second) return corner.third;
    if (top == corner.second && front == corner.third) return corner.first;
    if (top == corner.third && front == corner.first) return corner.second;
  }
  throw std::invalid_argument("Provided top and front faces are inconsistent!");
}

Face getRight(const Face& top, const Face& front) {
  return getOpposite(getLeft(top, front));
}

std::pair<RotationAxis, bool> getRotationAxis(const Face& face) {
  switch (face) {
    case U:
      return {RotationAxis::Y, false};
    case F:
      return {RotationAxis::Z, false};
    case R:
      return {RotationAxis::X, false};
    case B:
      return {RotationAxis::Z, true};
    case L:
      return {RotationAxis::X, true};
    case D:
      return {RotationAxis::Y, true};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStr(const Face& face) {
  switch (face) {
    case U:
      return "U";
    case F:
      return "F";
    case R:
      return "R";
    case B:
      return "B";
    case L:
      return "L";
    case D:
      return "D";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStrLower(const Face& face) {
  switch (face) {
    case U:
      return "u";
    case F:
      return "f";
    case R:
      return "r";
    case B:
      return "b";
    case L:
      return "l";
    case D:
      return "d";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<size_t, Face> parseFace(const std::string& str) {
  if (str.empty()) return {0, {}};
  switch (str[0]) {
    case 'U':
      return {1, U};
    case 'F':
      return {1, F};
    case 'R':
      return {1, R};
    case 'B':
      return {1, B};
    case 'L':
      return {1, L};
    case 'D':
      return {1, D};
    default:
      return {0, {}};
  }
}

std::pair<size_t, Face> parseWideFace(const std::string& str) {
  if (str.empty()) return {0, {}};
  switch (str[0]) {
    case 'u':
      return {1, U};
    case 'f':
      return {1, F};
    case 'r':
      return {1, R};
    case 'b':
      return {1, B};
    case 'l':
      return {1, L};
    case 'd':
      return {1, D};
    default:
      return {0, {}};
  }
}
