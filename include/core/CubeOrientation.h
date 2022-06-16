#pragma once

#include "CubeRotation.h"
#include "Face.h"
#include "Slice.h"
#include "Turn.h"
#include <cstddef>
#include <optional>
#include <utility>

struct CubeOrientation {
  Face top;    // the face that is now on top
  Face front;  // the face that is now in the front

  bool isValid() const;

  bool operator==(const CubeOrientation& other) const;

  [[nodiscard]] Face getRightFace() const;

  [[nodiscard]] Face getBackFace() const;

  [[nodiscard]] Face getLeftFace() const;

  [[nodiscard]] Face getBottomFace() const;

  static constexpr CubeOrientation identity() { return {Face::U, Face::F}; }

  static CubeOrientation random();

  [[nodiscard]] CubeOrientation inv() const;

  /**
   * @brief Applies the given CubeRotation to this CubeOrientation and returns
   * the resulting CubeOrientation
   */
  [[nodiscard]] CubeOrientation operator*(
      const CubeRotation& cubeRotation) const;

  void operator*=(const CubeRotation& cubeRotation);

  /**
   * Computes the CubeRotation(s) that would transform this CubeOrientation to
   * the identity CubeOrientation.
   */
  std::pair<std::optional<CubeRotation>, std::optional<CubeRotation>> solve()
      const;

  /**
   * @brief Converts the given Face in this CubeOrientation to the equivalent
   * Face in the identity CubeOrientation
   */
  [[nodiscard]] Face apply(const Face& face) const;

  /**
   * @brief Converts the given Slice in this CubeOrientation to the equivalent
   * Slice in the identity CubeOrientation. If the bool is true, then the Slice
   * is reversed
   */
  [[nodiscard]] std::pair<Slice, bool> apply(const Slice& slice) const;

  /**
   * @brief Converts the given Turn in this CubeOrientation to the equivalent
   * Turn in the identity CubeOrientation
   */
  [[nodiscard]] Turn apply(const Turn& turn) const;

  [[nodiscard]] CubeRotation apply(const CubeRotation& cubeRotation) const;
};

namespace std {
template <>
struct hash<CubeOrientation> {
  size_t operator()(const CubeOrientation& cube_orientation) const {
    static constexpr std::hash<Face> hasher{};
    return 31 * hasher(cube_orientation.top) + hasher(cube_orientation.front);
  }
};
}  // namespace std
