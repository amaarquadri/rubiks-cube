#pragma once

#include <string>

namespace blindsolving {
struct BlindsolvingMove {
  bool is_parity;
  bool is_edge;  // only valid if !is_parity
  char alg;      // only valid if !is_parity

  constexpr BlindsolvingMove() : is_parity(true) {}

  constexpr BlindsolvingMove(const bool& is_edge, const char& alg)
      : is_parity(false), is_edge(is_edge), alg(alg) {}

  BlindsolvingMove(const BlindsolvingMove& other);

  BlindsolvingMove& operator=(const BlindsolvingMove& other) = default;

  [[nodiscard]] bool operator==(const BlindsolvingMove& other) const;

  [[nodiscard]] bool operator!=(const BlindsolvingMove& other) const;

  [[nodiscard]] std::string toStr() const;

  constexpr ~BlindsolvingMove(){};
};
}  // namespace blindsolving
