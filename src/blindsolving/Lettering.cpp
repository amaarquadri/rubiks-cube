#include "Lettering.h"
#include "CornerLocation.h"
#include "EdgeLocation.h"
#include "Face.h"
#include <unordered_map>
#include <utility>

namespace blindsolving {
static std::unordered_map<EdgeLocation, char> getEdgeLettering() {
  using enum Face;
  static constexpr auto make =
      [](const Face& first, const Face& second, const char& c) consteval {
    return std::pair<const EdgeLocation, char>{EdgeLocation{first, second}, c};
  };
  return {make(U, B, 'A'), make(U, R, 'B'), make(U, F, 'C'), make(U, L, 'D'),
          make(F, U, 'E'), make(F, R, 'F'), make(F, D, 'G'), make(F, L, 'H'),
          make(R, U, 'I'), make(R, B, 'J'), make(R, D, 'K'), make(R, F, 'L'),
          make(B, U, 'M'), make(B, L, 'N'), make(B, D, 'O'), make(B, R, 'P'),
          make(L, U, 'Q'), make(L, F, 'R'), make(L, D, 'S'), make(L, B, 'T'),
          make(D, F, 'U'), make(D, R, 'V'), make(D, B, 'W'), make(D, L, 'Z')};
}

static std::unordered_map<CornerLocation, char> getCornerLettering() {
  using enum Face;
  static constexpr auto make = [](const Face& first, const Face& second,
                                  const Face& third, const char& c) consteval {
    return std::pair<const CornerLocation, char>{
        CornerLocation{first, second, third}, c};
  };
  return {make(U, L, B, 'A'), make(U, B, R, 'B'), make(U, R, F, 'C'),
          make(U, F, L, 'D'), make(F, L, U, 'E'), make(F, U, R, 'F'),
          make(F, R, D, 'G'), make(F, D, L, 'H'), make(R, F, U, 'I'),
          make(R, U, B, 'J'), make(R, B, D, 'K'), make(R, D, F, 'L'),
          make(B, R, U, 'M'), make(B, U, L, 'N'), make(B, L, D, 'O'),
          make(B, D, R, 'P'), make(L, B, U, 'Q'), make(L, U, F, 'R'),
          make(L, F, D, 'S'), make(L, D, B, 'T'), make(D, L, F, 'U'),
          make(D, F, R, 'V'), make(D, R, B, 'W'), make(D, B, L, 'Z')};
}

template <typename K, typename V>
static std::unordered_map<V, K> reverseMap(
    const std::unordered_map<K, V>& map) {
  std::unordered_map<V, K> reverse_map;
  for (const auto& [k, v] : map) reverse_map.insert({v, k});
  return reverse_map;
}

const std::unordered_map<EdgeLocation, char> EDGE_LETTERING =
    getEdgeLettering();  // NOLINT(cert-err58-cpp)
const std::unordered_map<CornerLocation, char> CORNER_LETTERING =
    getCornerLettering();  // NOLINT(cert-err58-cpp)
const std::unordered_map<char, EdgeLocation> REVERSE_EDGE_LETTERING =
    reverseMap(EDGE_LETTERING);  // NOLINT(cert-err58-cpp)
const std::unordered_map<char, CornerLocation> REVERSE_CORNER_LETTERING =
    reverseMap(CORNER_LETTERING);  // NOLINT(cert-err58-cpp)

char flipEdge(const char& edge) {
  return EDGE_LETTERING.at(REVERSE_EDGE_LETTERING.at(edge).flip());
}

char rotateClockwise(const char& corner) {
  return CORNER_LETTERING.at(
      REVERSE_CORNER_LETTERING.at(corner).rotateClockwise());
}

char rotateCounterclockwise(const char& corner) {
  return CORNER_LETTERING.at(
      REVERSE_CORNER_LETTERING.at(corner).rotateCounterclockwise());
}
}  // namespace blindsolving
