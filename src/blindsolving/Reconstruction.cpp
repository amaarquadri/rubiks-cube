#include "Reconstruction.h"

namespace blindsolving {
Reconstruction::Reconstruction(const Reconstruction& other) { *this = other; }

Reconstruction& Reconstruction::operator=(const Reconstruction& other) {
  solve_data.resize(other.solve_data.size());
  std::copy(other.solve_data.begin(), other.solve_data.end(),
            solve_data.begin());
  return *this;
}

size_t Reconstruction::length() const { return solve_data.size(); }

std::string Reconstruction::toStr() const {
  unsigned int consumed = 0;
  std::string result;
  while (consumed < length()) {
    if (!solve_data[consumed].is_parsed) {
      result += "Moves: [" + solve_data[consumed].moves.toStr() + "], ";
      consumed++;
      continue;
    }
    if (solve_data[consumed].is_parity) {
      result += "Parity, ";
      consumed++;
      continue;
    }
    if (solve_data[consumed].is_edge) {
      result += "Edges: [";
      while (consumed < length() && solve_data[consumed].is_parsed &&
             !solve_data[consumed].is_parity && solve_data[consumed].is_edge) {
        result += std::string(1, solve_data[consumed].alg) + " ";
        consumed++;
      }
      result += "], ";
    } else {
      result += "Corners: [";
      while (consumed < length() && solve_data[consumed].is_parsed &&
             !solve_data[consumed].is_parity && !solve_data[consumed].is_edge) {
        result += std::string(1, solve_data[consumed].alg) + " ";
        consumed++;
      }
      result += "], ";
    }
  }
  return result;
}

SolveData Reconstruction::operator[](const size_t& index) const {
  // TODO: why is this constructor needed?
  return SolveData{solve_data[index]};
}

SolveData& Reconstruction::operator[](const size_t& index) {
  return solve_data[index];
}

SolveData& Reconstruction::front() { return solve_data.front(); }

const SolveData& Reconstruction::front() const { return solve_data.front(); }

SolveData& Reconstruction::back() { return solve_data.back(); }

const SolveData& Reconstruction::back() const { return solve_data.back(); }
}  // namespace blindsolving
