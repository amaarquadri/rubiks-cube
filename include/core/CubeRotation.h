#pragma once

#include "RotationAmount.h"
#include "RotationAxis.h"
#include <string>
#include <utility>

struct CubeRotation {
  RotationAxis rotationAxis;
  RotationAmount rotationAmount;

  [[nodiscard]] CubeRotation inv() const;

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed
   * CubeRotation. If it was not possible to parse a CubeRotation, then the
   * number of characters consumed will be zero
   */
  static std::pair<int, CubeRotation> parse(const std::string& str);
};
