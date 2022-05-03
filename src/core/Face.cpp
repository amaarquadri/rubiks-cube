#include "Face.h"
#include "CornerLocation.h"
#include "Cube.h"
#include <stdexcept>

Face getOpposite(const Face& face) {
  switch (face) {
    case U:
      return D;
    case F:
      return B;
    case R:
      return L;
    case B:
      return F;
    case L:
      return R;
    case D:
      return U;
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

std::pair<int, Face> parseFace(const std::string& str) {
  if (str.empty()) return {0, {}};
  char first_char = str[0];
  if (first_char == 'U')
    return {1, U};
  else if (first_char == 'F')
    return {1, F};
  else if (first_char == 'R')
    return {1, R};
  else if (first_char == 'B')
    return {1, B};
  else if (first_char == 'L')
    return {1, L};
  else if (first_char == 'D')
    return {1, D};
  else
    return {0, {}};
}
