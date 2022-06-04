#include "Face.h"
#include "CornerLocation.h"
#include "Cube.h"
#include "RotationAxis.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

Face getOpposite(const Face& face) {
  switch (face) {
    case Face::U:
      return Face::D;
    case Face::F:
      return Face::B;
    case Face::R:
      return Face::L;
    case Face::B:
      return Face::F;
    case Face::L:
      return Face::R;
    case Face::D:
      return Face::U;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

Face getLeft(const Face& top, const Face& front) {
  for (const CornerLocation& corner : Cube::CORNER_LOCATION_ORDER) {
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
    case Face::U:
      return {RotationAxis::Y, false};
    case Face::F:
      return {RotationAxis::Z, false};
    case Face::R:
      return {RotationAxis::X, false};
    case Face::B:
      return {RotationAxis::Z, true};
    case Face::L:
      return {RotationAxis::X, true};
    case Face::D:
      return {RotationAxis::Y, true};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStr(const Face& face) {
  switch (face) {
    case Face::U:
      return "U";
    case Face::F:
      return "F";
    case Face::R:
      return "R";
    case Face::B:
      return "B";
    case Face::L:
      return "L";
    case Face::D:
      return "D";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStrLower(const Face& face) {
  switch (face) {
    case Face::U:
      return "u";
    case Face::F:
      return "f";
    case Face::R:
      return "r";
    case Face::B:
      return "b";
    case Face::L:
      return "l";
    case Face::D:
      return "d";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<size_t, Face> parseFace(const std::string& str) {
  if (str.empty()) return {0, {}};
  const char& first_char = str[0];
  if (first_char == 'U')
    return {1, Face::U};
  else if (first_char == 'F')
    return {1, Face::F};
  else if (first_char == 'R')
    return {1, Face::R};
  else if (first_char == 'B')
    return {1, Face::B};
  else if (first_char == 'L')
    return {1, Face::L};
  else if (first_char == 'D')
    return {1, Face::D};
  else
    return {0, {}};
}

std::pair<size_t, Face> parseWideFace(const std::string& str) {
  if (str.empty()) return {0, {}};
  const char& first_char = str[0];
  if (first_char == 'u')
    return {1, Face::U};
  else if (first_char == 'f')
    return {1, Face::F};
  else if (first_char == 'r')
    return {1, Face::R};
  else if (first_char == 'b')
    return {1, Face::B};
  else if (first_char == 'l')
    return {1, Face::L};
  else if (first_char == 'd')
    return {1, Face::D};
  else
    return {0, {}};
}
