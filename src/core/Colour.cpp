#include "Colour.h"
#include "Cube.h"
#include <stdexcept>

Colour getOpposite(const Colour& colour) {
  switch (colour) {
    case Colour::WHITE:
      return Colour::YELLOW;
    case Colour::GREEN:
      return Colour::BLUE;
    case Colour::RED:
      return Colour::ORANGE;
    case Colour::BLUE:
      return Colour::GREEN;
    case Colour::ORANGE:
      return Colour::RED;
    case Colour::YELLOW:
      return Colour::WHITE;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

Colour getLeft(const Colour& top, const Colour& front) {
  for (CornerPiece corner : Cube::STARTING_CORNER_PIECES) {
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
    case Colour::WHITE:
      return "W";
    case Colour::GREEN:
      return "G";
    case Colour::RED:
      return "R";
    case Colour::BLUE:
      return "B";
    case Colour::ORANGE:
      return "O";
    case Colour::YELLOW:
      return "Y";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}
