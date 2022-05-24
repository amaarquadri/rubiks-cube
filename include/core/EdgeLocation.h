#pragma once

#include "Face.h"

struct EdgeLocation {
  Face first;
  Face second;

  [[nodiscard]] bool operator==(const EdgeLocation& other) const;

  [[nodiscard]] bool operator!=(const EdgeLocation& other) const;

  [[nodiscard]] EdgeLocation flip() const;
};

namespace std {
template <>
struct hash<EdgeLocation> {
  size_t operator()(const EdgeLocation& edgeLocation) const {
    static constexpr std::hash<Face> hasher{};
    return 31 * hasher(edgeLocation.first) + hasher(edgeLocation.second);
  }
};
}  // namespace std
