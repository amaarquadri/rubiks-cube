#pragma once

#include "Face.h"
#include "RandomUtils.h"
#include "RotationAmount.h"
#include <cstddef>
#include <string>
#include <utility>

struct Turn {
  Face face;
  RotationAmount rotation_amount;

  constexpr Turn() : face(Face{}), rotation_amount(RotationAmount{}) {}

  constexpr Turn(const Face& face, const RotationAmount& rotationAmount)
      : face(face), rotation_amount(rotationAmount) {}

  bool operator==(const Turn& other) const = default;

  /**
   * @return A random Turn that is neither a slice turn nor a wide turn.
   */
  static Turn random() {
    const size_t rand = utility::randomInt<18>();
    return Turn{static_cast<Face>(rand % 6),
                // add 1 to exclude RotationAmount::None
                static_cast<RotationAmount>(1 + rand / 6)};
  }

  [[nodiscard]] constexpr Turn inv() const {
    return Turn{face, -rotation_amount};
  }

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed Turn. If it
   * was not possible to parse a Turn, then the number of characters consumed
   * will be zero.
   */
  static std::pair<size_t, Turn> parse(const std::string& str);
};
