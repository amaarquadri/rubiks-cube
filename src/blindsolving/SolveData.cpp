#include "SolveData.h"

namespace blindsolving {
SolveData::SolveData(const SolveData& other) { *this = other; }

bool SolveData::operator==(const SolveData& other) const {
  if (is_parsed != other.is_parsed) return false;
  if (!is_parsed) return moves == other.moves;
  // otherwise, both must be parsed

  return blindsolving_move == other.blindsolving_move;
}

bool SolveData::operator!=(const SolveData& other) const {
  return !(*this == other);
}

std::string SolveData::toStr() const {
  if (is_parsed) {
    return blindsolving_move.toStr();
  } else {
    return "Unknown Moves: [" + moves.toStr() + "]";
  }
}
}  // namespace blindsolving
