#include "Reconstruction.h"
#include <cstddef>
#include <string>

namespace blindsolving {
std::string Reconstruction::toStr() const {
  size_t consumed = 0;
  std::string result;
  while (consumed < size()) {
    if (!(*this)[consumed].is_parsed) {
      result += (*this)[consumed].toStr() + ", ";
      consumed++;
      continue;
    }
    if ((*this)[consumed].blindsolving_move.is_parity) {
      result += "Parity, ";
      consumed++;
      continue;
    }
    if ((*this)[consumed].blindsolving_move.is_edge) {
      result += "Edges: [";
      while (consumed < size() && (*this)[consumed].is_parsed &&
             !(*this)[consumed].blindsolving_move.is_parity &&
             (*this)[consumed].blindsolving_move.is_edge) {
        result += std::string(1, (*this)[consumed].blindsolving_move.alg) + " ";
        consumed++;
      }
      result += "], ";
    } else {
      result += "Corners: [";
      while (consumed < size() && (*this)[consumed].is_parsed &&
             !(*this)[consumed].blindsolving_move.is_parity &&
             !(*this)[consumed].blindsolving_move.is_edge) {
        result += std::string(1, (*this)[consumed].blindsolving_move.alg) + " ";
        consumed++;
      }
      result += "], ";
    }
  }
  return result;
}
}  // namespace blindsolving
