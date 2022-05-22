#include "Lettering.h"
#include "Cube.h"

namespace blindsolving {
static std::unordered_map<EdgeLocation, char> getEdgeLettering() {
  std::unordered_map<EdgeLocation, char> edge_lettering;
  const std::string edge_labels = "ABCDFJNRUVWZ";
  const std::string flipped_edge_labels = "MIEQLPTHGKOS";
  for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size(); i++) {
    EdgeLocation location = Cube::EDGE_LOCATION_ORDER[i];
    edge_lettering.insert({location, edge_labels[i]});
    edge_lettering.insert({location.flip(), flipped_edge_labels[i]});
  }
  return edge_lettering;
}

static std::unordered_map<CornerLocation, char> getCornerLettering() {
  std::unordered_map<CornerLocation, char> corner_lettering;
  const std::string corner_labels = "ABCDUVWZ";
  const std::string clockwise_corner_labels = "NJFRHLPT";
  const std::string counterclockwise_corner_labels = "QMIESGKO";
  for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size(); i++) {
    CornerLocation location = Cube::CORNER_LOCATION_ORDER[i];
    corner_lettering.insert({location, corner_labels[i]});
    corner_lettering.insert(
        {location.rotateClockwise(), clockwise_corner_labels[i]});
    corner_lettering.insert(
        {location.rotateCounterClockwise(), counterclockwise_corner_labels[i]});
  }
  return corner_lettering;
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

char rotateCounterClockwise(const char& corner) {
  return CORNER_LETTERING.at(
      REVERSE_CORNER_LETTERING.at(corner).rotateCounterClockwise());
}
}  // namespace blindsolving