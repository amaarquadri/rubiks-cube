#pragma once

#include "BlindsolvingMove.h"
#include "Cube.h"
#include <string>
#include <vector>

namespace blindsolving {
class BlindsolvingReconstruction : public std::vector<BlindsolvingMove> {
 public:
  void applyTo(Cube& cube) const;

  std::string toStr() const;
};
}  // namespace blindsolving

namespace std {
template <>
struct hash<blindsolving::BlindsolvingReconstruction> {
  size_t operator()(
      const blindsolving::BlindsolvingReconstruction& recon) const {
    static constexpr hash<blindsolving::BlindsolvingMove> hasher{};
    size_t val = 1249;  // arbitrary large prime
    for (const auto& move : recon) val = 31 * val + hasher(move);
    return val;
  }
};
}  // namespace std