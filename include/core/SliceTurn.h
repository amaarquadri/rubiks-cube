#pragma once

#include "CubeRotation.h"
#include "RotationAmount.h"
#include "Slice.h"
#include "Turn.h"
#include <cstddef>
#include <string>
#include <tuple>
#include <utility>

struct SliceTurn {
  Slice slice;
  RotationAmount rotation_amount;

  constexpr SliceTurn() : slice(Slice{}), rotation_amount(RotationAmount{}) {}

  constexpr SliceTurn(const Slice& slice, const RotationAmount& rotationAmount)
      : slice(slice), rotation_amount(rotationAmount) {}

  [[nodiscard]] constexpr SliceTurn inv() const {
    return SliceTurn{slice, -rotation_amount};
  }

  [[nodiscard]] std::string toStr() const;

  [[nodiscard]] std::tuple<Turn, Turn, CubeRotation> expand() const;

  /**
   * @brief Returns the number of characters consumed and the parsed
   * SliceTurn. If it was not possible to parse a SliceTurn, then the number
   * of characters consumed will be zero.
   */
  static std::pair<size_t, SliceTurn> parse(const std::string& str);
};
