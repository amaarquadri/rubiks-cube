#include "BlindsolvingMove.h"
#include "Blindsolving.h"
#include "Cube.h"

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

void BlindsolvingMove::applyTo(Cube& cube) const {
  if (is_parity)
    cube.apply(PARITY_ALG);
  else if (is_edge)
    cube.apply(EDGE_ALGS.at(alg));
  else
    cube.apply(CORNER_ALGS.at(alg));
}
}  // namespace blindsolving
