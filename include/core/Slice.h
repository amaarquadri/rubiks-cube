#pragma once

#include "Face.h"
#include "RotationAxis.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

enum class Slice : uint8_t { M, E, S };

/**
 * @return The face that has the same rotation axis and direction as the given
 * Slice
 */
constexpr Face getRotationFace(const Slice& slice) {
  switch (slice) {
    case Slice::M:
      return Face::L;
    case Slice::E:
      return Face::D;
    case Slice::S:
      return Face::F;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

/**
 * @brief Computes the RotationAxis that results from applying this Slice with a
 * RotationAmount of Clockwise. If the bool is true, then the RotationAxis is in
 * the opposite direction
 */
constexpr std::pair<RotationAxis, bool> getRotationAxis(const Slice& slice) {
  switch (slice) {
    case Slice::M:
      return {RotationAxis::X, true};
    case Slice::E:
      return {RotationAxis::Y, true};
    case Slice::S:
      return {RotationAxis::Z, false};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

/**
 * @breif Computes the slice that has the same rotation direction as the given
 * Face. If the bool is true, then the slice is the opposite direction
 */
constexpr std::pair<Slice, bool> fromRotationFace(const Face& face) {
  switch (face) {
    case Face::L:
      return {Slice::M, false};
    case Face::R:
      return {Slice::M, true};
    case Face::D:
      return {Slice::E, false};
    case Face::U:
      return {Slice::E, true};
    case Face::F:
      return {Slice::S, false};
    case Face::B:
      return {Slice::S, true};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::string toStr(const Slice& slice);

/**
 * @brief Returns the number of characters consumed and the parsed Slice. If it
 * was not possible to parse a Slice, then the number of characters consumed
 * will be zero.
 */
std::pair<size_t, Slice> parseSlice(const std::string& str);
