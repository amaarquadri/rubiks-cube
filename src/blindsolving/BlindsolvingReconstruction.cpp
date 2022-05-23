#include "BlindsolvingReconstruction.h"
#include <string>

namespace blindsolving {
void BlindsolvingReconstruction::applyTo(Cube& cube) const {
  for (const BlindsolvingMove& move : *this) move.applyTo(cube);
}

std::string BlindsolvingReconstruction::toStr() const {
  unsigned int consumed = 0;
  std::string result;
  while (consumed < size()) {
    if ((*this)[consumed].is_parity) {
      result += "Parity, ";
      consumed++;
      continue;
    }
    if ((*this)[consumed].is_edge) {
      result += "Edges: [";
      while (consumed < size() && !(*this)[consumed].is_parity &&
             (*this)[consumed].is_edge) {
        result += std::string(1, (*this)[consumed].alg) + " ";
        consumed++;
      }
      result += "], ";
    } else {
      result += "Corners: [";
      while (consumed < size() && !(*this)[consumed].is_parity &&
             !(*this)[consumed].is_edge) {
        result += std::string(1, (*this)[consumed].alg) + " ";
        consumed++;
      }
      result += "], ";
    }
  }
  return result;
}
}  // namespace blindsolving
