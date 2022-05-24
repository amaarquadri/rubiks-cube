#include "ReconstructionIterator.h"
#include "BlindsolvingReconstruction.h"

namespace blindsolving {
static size_t calculateLength(
    const std::vector<char>& first_edge_cycle,
    const std::vector<std::vector<char>>& edge_cycles, const bool& has_parity,
    const std::vector<char>& first_corner_cycle,
    const std::vector<std::vector<char>>& corner_cycles) {
  size_t length = first_edge_cycle.size() + first_corner_cycle.size();
  if (has_parity) ++length;

  // add one extra to close each additional cycle
  for (const auto& cycle : edge_cycles) length += cycle.size() + 1;
  for (const auto& cycle : corner_cycles) length += cycle.size() + 1;
  return length;
}

ReconstructionIterator::ReconstructionIterator(
    const std::vector<char>& first_edge_cycle,
    const std::vector<std::vector<char>>& edge_cycles,
    const std::vector<bool>& are_even, const bool& has_parity,
    const std::vector<char>& first_corner_cycle,
    const std::vector<std::vector<char>>& corner_cycles,
    const std::vector<CornerRotationAmount>& rotation_amounts)
    : first_edge_cycle(first_edge_cycle),
      edge_iterator(EdgeCycleSequenceIterator{edge_cycles, are_even}),
      has_parity(has_parity),
      first_corner_cycle(first_corner_cycle),
      corner_iterator(
          CornerCycleSequenceIterator{corner_cycles, rotation_amounts}),
      length(calculateLength(first_edge_cycle, edge_cycles, has_parity,
                             first_corner_cycle, corner_cycles)) {}

bool ReconstructionIterator::operator++() {
  /**
   * This takes advantage of short-circuiting.
   * Note that the order is chosen for efficiency:
   * Let E be the number of iterations of ++edge_iterator before it cycles and
   * let C be the number of iterations of ++corner_iterator before it cycles.
   * Then this solution will perform E * C + C increments whereas flipping the
   * order will perform E * C + E increments. This is more efficient in general
   * since typically E > C.
   */
  return ++edge_iterator || ++corner_iterator;
}

static char swapIfNecessary(const char& chr) {
  switch (chr) {
    case 'C':
      return 'W';
    case 'W':
      return 'C';
    case 'E':
      return 'O';
    case 'O':
      return 'E';
    default:
      return chr;
  }
}

BlindsolvingReconstruction ReconstructionIterator::operator*() const {
  BlindsolvingReconstruction reconstruction;
  reconstruction.reserve(length);
  for (const char& edge : first_edge_cycle)
    reconstruction.emplace_back(true, edge);
  const std::vector<char> edge_moves = *edge_iterator;
  for (const char& edge : edge_moves) reconstruction.emplace_back(true, edge);
  for (size_t i = 1; i < reconstruction.size(); i += 2)
    reconstruction[i].alg = swapIfNecessary(reconstruction[i].alg);
  if (has_parity) reconstruction.emplace_back();
  for (const char& corner : first_corner_cycle)
    reconstruction.emplace_back(false, corner);
  std::vector<char> corner_moves = *corner_iterator;
  for (const char& corner : corner_moves)
    reconstruction.emplace_back(false, corner);
  return reconstruction;
}

size_t ReconstructionIterator::getPeriod() const {
  return edge_iterator.getPeriod() * corner_iterator.getPeriod();
}

void ReconstructionIterator::reset() {
  edge_iterator.reset();
  corner_iterator.reset();
}
}  // namespace blindsolving
