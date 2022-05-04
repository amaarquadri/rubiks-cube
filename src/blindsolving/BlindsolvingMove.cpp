#include "BlindsolvingMove.h"

namespace blindsolving {
BlindsolvingMove::BlindsolvingMove(const BlindsolvingMove& other) {
  *this = other;
}

bool BlindsolvingMove::operator==(const BlindsolvingMove& other) const {
  if (is_parity != other.is_parity) return false;
  if (is_parity) return true;  // both are parity
  // otherwise, neither are parity

  if (is_edge != other.is_edge) return false;
  // otherwise, both must be the same piece type

  return alg == other.alg;
}

bool BlindsolvingMove::operator!=(const BlindsolvingMove& other) const {
  return !(*this == other);
}

std::string BlindsolvingMove::toStr() const {
  if (is_parity) return "Parity";
  return (is_edge ? "Edge: " : "Corner: ") + std::string(1, alg);
}
}  // namespace blindsolving
