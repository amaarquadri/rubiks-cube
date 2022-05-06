#pragma once

#include "Algorithm.h"
#include "BlindsolvingMove.h"
#include <string>

namespace blindsolving {
struct SolveData {
  Algorithm moves;
  bool is_parsed;
  BlindsolvingMove blindsolving_move{};  // only valid if is_parsed

  SolveData() : is_parsed(true) {}

  explicit SolveData(const Algorithm& moves, const bool& is_edge,
                     const char& alg)
      : moves(moves),
        is_parsed(true),
        blindsolving_move(BlindsolvingMove{is_edge, alg}) {}

  explicit SolveData(const Algorithm& moves) : is_parsed(false), moves(moves) {}

  SolveData(const SolveData& other);

  SolveData& operator=(const SolveData& other) = default;

  [[nodiscard]] bool operator==(const SolveData& other) const;

  [[nodiscard]] bool operator!=(const SolveData& other) const;

  [[nodiscard]] std::string toStr() const;

  ~SolveData(){};
};
}  // namespace blindsolving
