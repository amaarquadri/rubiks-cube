#pragma once

#include "RotationAmount.h"
#include "RotationAxis.h"
#include <string>
#include <utility>

struct CubeRotation {
  RotationAxis rotation_axis;
  RotationAmount rotation_amount;

  [[nodiscard]] constexpr CubeRotation inv() const {
    return {rotation_axis, -rotation_amount};
  }

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed
   * CubeRotation. If it was not possible to parse a CubeRotation, then the
   * number of characters consumed will be zero.
   */
  static std::pair<size_t, CubeRotation> parse(const std::string& str);
};
