#pragma once

#include "Face.h"
#include "RotationAmount.h"
#include "Slice.h"
#include <string>
#include <utility>

struct Turn {
  bool is_slice_turn;
  union {
    Face face;
    Slice slice;
  };
  RotationAmount rotationAmount;

  Turn()
      : is_slice_turn(false), face(Face{}), rotationAmount(RotationAmount{}) {}

  Turn(const Face& face, const RotationAmount& rotationAmount)
      : is_slice_turn(false), face(face), rotationAmount(rotationAmount) {}

  Turn(const Slice& slice, const RotationAmount& rotationAmount)
      : is_slice_turn(true), slice(slice), rotationAmount(rotationAmount) {}

  [[nodiscard]] Turn inv() const;

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed Turn. If it
   * was not possible to parse a Turn, then the number of characters consumed
   * will be zero
   */
  static std::pair<int, Turn> parse(const std::string& str);
};
