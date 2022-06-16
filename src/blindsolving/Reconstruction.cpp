#include "Reconstruction.h"
#include <cstddef>
#include <string>

namespace blindsolving {
std::string toStr(const Reconstruction& reconstruction) {
  size_t consumed = 0;
  std::string result;
  while (consumed < reconstruction.size()) {
    if (!reconstruction[consumed].is_parsed) {
      result += reconstruction[consumed].toStr() + ", ";
      consumed++;
      continue;
    }
    if (reconstruction[consumed].blindsolving_move.is_parity) {
      result += "Parity, ";
      consumed++;
      continue;
    }
    if (reconstruction[consumed].blindsolving_move.is_edge) {
      result += "Edges: [";
      while (consumed < reconstruction.size() &&
             reconstruction[consumed].is_parsed &&
             !reconstruction[consumed].blindsolving_move.is_parity &&
             reconstruction[consumed].blindsolving_move.is_edge) {
        result +=
            std::string(1, reconstruction[consumed].blindsolving_move.alg) +
            " ";
        consumed++;
      }
      result += "], ";
    } else {
      result += "Corners: [";
      while (consumed < reconstruction.size() &&
             reconstruction[consumed].is_parsed &&
             !reconstruction[consumed].blindsolving_move.is_parity &&
             !reconstruction[consumed].blindsolving_move.is_edge) {
        result +=
            std::string(1, reconstruction[consumed].blindsolving_move.alg) +
            " ";
        consumed++;
      }
      result += "], ";
    }
  }
  return result;
}
}  // namespace blindsolving
