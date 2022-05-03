#pragma once

#include "SolveData.h"
#include <string>

namespace blindsolving {
struct Reconstruction {
  std::vector<SolveData> solve_data;

  Reconstruction() = default;

  explicit Reconstruction(std::vector<SolveData> solve_data)
      : solve_data(std::move(solve_data)) {}

  Reconstruction(const Reconstruction& other);

  Reconstruction& operator=(const Reconstruction& other);

  [[nodiscard]] size_t length() const;

  [[nodiscard]] std::string toStr() const;

  [[nodiscard]] SolveData operator[](const size_t& index) const;

  [[nodiscard]] SolveData& operator[](const size_t& index);

  [[nodiscard]] SolveData& front();

  [[nodiscard]] const SolveData& front() const;

  [[nodiscard]] SolveData& back();

  [[nodiscard]] const SolveData& back() const;
};
}  // namespace blindsolving
