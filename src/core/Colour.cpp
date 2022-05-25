#include "Colour.h"
#include "CornerPiece.h"
#include "Cube.h"
#include <stdexcept>
#include <string>

Colour getOpposite(const Colour& colour) {
  switch (colour) {
    case Colour::White:
      return Colour::Yellow;
    case Colour::Green:
      return Colour::Blue;
    case Colour::Red:
      return Colour::Orange;
    case Colour::Blue:
      return Colour::Green;
    case Colour::Orange:
      return Colour::Red;
    case Colour::Yellow:
      return Colour::White;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

Colour getLeft(const Colour& top, const Colour& front) {
  for (const CornerPiece& corner : Cube::STARTING_CORNER_PIECES) {
    if (top == corner.first && front == corner.second) return corner.third;
    if (top == corner.second && front == corner.third) return corner.first;
    if (top == corner.third && front == corner.first) return corner.second;
  }
  throw std::invalid_argument(
      "Provided top and front colours are inconsistent!");
}

Colour getRight(const Colour& top, const Colour& front) {
  return getOpposite(getLeft(top, front));
}

std::string toStr(const Colour& colour) {
  switch (colour) {
    case Colour::White:
      return "W";
    case Colour::Green:
      return "G";
    case Colour::Red:
      return "R";
    case Colour::Blue:
      return "B";
    case Colour::Orange:
      return "O";
    case Colour::Yellow:
      return "Y";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}
