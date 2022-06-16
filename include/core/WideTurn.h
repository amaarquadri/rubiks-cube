#pragma once

#include "CubeRotation.h"
#include "Face.h"
#include "RotationAmount.h"
#include "Turn.h"
#include <cstddef>
#include <string>
#include <utility>

struct WideTurn {
  Face face;
  RotationAmount rotation_amount;

  constexpr WideTurn() : face(Face{}), rotation_amount(RotationAmount{}) {}

  constexpr WideTurn(const Face& face, const RotationAmount& rotationAmount)
      : face(face), rotation_amount(rotationAmount) {}

  [[nodiscard]] constexpr WideTurn inv() const {
    return WideTurn{face, -rotation_amount};
  }

  [[nodiscard]] std::string toStr() const;

  [[nodiscard]] std::pair<Turn, CubeRotation> expand() const;

  /**
   * @brief Returns the number of characters consumed and the parsed WideTurn.
   * If it was not possible to parse a WideTurn, then the number of characters
   * consumed will be zero.
   */
  static std::pair<size_t, WideTurn> parse(const std::string& str);
};
