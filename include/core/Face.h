#pragma once

#include "RotationAxis.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

enum class Face : uint8_t { U, F, R, B, L, D };

constexpr Face getOpposite(const Face& face) {
  using enum Face;
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

// TODO: make these functions constexpr
Face getLeft(const Face& top, const Face& front);

Face getRight(const Face& top, const Face& right);

/**
 * @brief Computes the RotationAxis that results from applying a wide turn on
 * this Face with a RotationAmount of Clockwise. If the bool is true, then the
 * RotationAxis is in the opposite direction.
 */
constexpr std::pair<RotationAxis, bool> getRotationAxis(const Face& face) {
  using enum Face;
  using enum RotationAxis;
  switch (face) {
    case U:
      return {Y, false};
    case F:
      return {Z, false};
    case R:
      return {X, false};
    case B:
      return {Z, true};
    case L:
      return {X, true};
    case D:
      return {Y, true};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStr(const Face& face);

std::string toStrLower(const Face& face);

/**
 * @brief Returns the number of characters consumed and the parsed Face. If it
 * was not possible to parse a Face, then the number of characters consumed will
 * be zero.
 */
constexpr std::pair<size_t, Face> parseFace(const std::string& str) {
  using enum Face;
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

constexpr std::pair<size_t, Face> parseWideFace(const std::string& str) {
  using enum Face;
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
