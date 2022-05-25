#include "Face.h"
#include "CornerLocation.h"
#include "Cube.h"
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
  for (CornerLocation corner : Cube::CORNER_LOCATION_ORDER) {
    if (top == corner.first && front == corner.second) return corner.third;
    if (top == corner.second && front == corner.third) return corner.first;
    if (top == corner.third && front == corner.first) return corner.second;
  }
  throw std::invalid_argument("Provided top and front faces are inconsistent!");
}

Face getRight(const Face& top, const Face& front) {
  return getOpposite(getLeft(top, front));
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
