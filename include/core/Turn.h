#pragma once

#include "Face.h"
#include "RotationAmount.h"
#include "Slice.h"
#include <string>
#include <utility>

struct Turn {
  // only one of the following two flags can be true
  bool is_wide_turn;
  bool is_slice_turn;
  union {
    Face face;
    Slice slice;
  };
  RotationAmount rotation_amount;

  Turn()
      : is_wide_turn(false),
        is_slice_turn(false),
        face(Face{}),
        rotation_amount(RotationAmount{}) {}

  Turn(const Face& face, const RotationAmount& rotationAmount)
      : is_wide_turn(false),
        is_slice_turn(false),
        face(face),
        rotation_amount(rotationAmount) {}

  Turn(const Slice& slice, const RotationAmount& rotationAmount)
      : is_wide_turn(false),
        is_slice_turn(true),
        slice(slice),
        rotation_amount(rotationAmount) {}

  static Turn makeWide(const Face& face, const RotationAmount& rotation_amount) {
    Turn t{face, rotation_amount};
    t.is_wide_turn = true;
    return t;
  }

  [[nodiscard]] Turn inv() const;

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed Turn. If it
   * was not possible to parse a Turn, then the number of characters consumed
   * will be zero.
   */
  static std::pair<size_t, Turn> parse(const std::string& str);
};
