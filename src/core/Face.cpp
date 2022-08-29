#include "Face.h"
#include "CornerLocation.h"
#include "Cube.h"
#include <stdexcept>
#include <string>

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
