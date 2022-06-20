#pragma once

#include "RotationAxis.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

enum class Face : uint8_t { U, F, R, B, L, D };

constexpr Face getOpposite(const Face& face) {
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

// TODO: make these functions constexpr
Face getLeft(const Face& top, const Face& front);

Face getRight(const Face& top, const Face& right);

/**
 * @brief Computes the RotationAxis that results from applying a wide turn on
 * this Face with a RotationAmount of Clockwise. If the bool is true, then the
 * RotationAxis is in the opposite direction.
 */
std::pair<RotationAxis, bool> getRotationAxis(const Face& face);

std::string toStr(const Face& face);

std::string toStrLower(const Face& face);

/**
 * @brief Returns the number of characters consumed and the parsed Face. If it
 * was not possible to parse a Face, then the number of characters consumed will
 * be zero.
 */
std::pair<size_t, Face> parseFace(const std::string& str);

std::pair<size_t, Face> parseWideFace(const std::string& str);
