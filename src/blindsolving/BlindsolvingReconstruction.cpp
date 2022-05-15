#include "BlindsolvingReconstruction.h"
#include <string>

namespace blindsolving {
std::string toStr(const BlindsolvingReconstruction& reconstruction) {
  unsigned int consumed = 0;
  std::string result;
  while (consumed < reconstruction.size()) {
    if (reconstruction[consumed].is_parity) {
      result += "Parity, ";
      consumed++;
      continue;
    }
    if (reconstruction[consumed].is_edge) {
      result += "Edges: [";
      while (consumed < reconstruction.size() &&
             !reconstruction[consumed].is_parity &&
             reconstruction[consumed].is_edge) {
        result += std::string(1, reconstruction[consumed].alg) + " ";
        consumed++;
      }
      result += "], ";
    } else {
      result += "Corners: [";
      while (consumed < reconstruction.size() &&
             !reconstruction[consumed].is_parity &&
             !reconstruction[consumed].is_edge) {
        result += std::string(1, reconstruction[consumed].alg) + " ";
        consumed++;
      }
      result += "], ";
    }
  }
  return result;
}
}  // namespace blindsolving
