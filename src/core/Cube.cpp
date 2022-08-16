#include "Cube.h"
#include "Permutation.h"
#include "RandomUtils.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>

EdgePieceProxy Cube::operator[](const EdgeLocation& edge_location) {
  const EdgeLocation flipped_location = edge_location.flip();
  for (size_t i = 0; i < EDGE_LOCATION_ORDER.size(); ++i) {
    if (EDGE_LOCATION_ORDER[i] == edge_location) return {edges[i], false};
    if (EDGE_LOCATION_ORDER[i] == flipped_location) return {edges[i], true};
  }
  throw std::invalid_argument("EdgeLocation not found!");
}

ConstEdgePieceProxy Cube::operator[](const EdgeLocation& edge_location) const {
  const EdgeLocation flipped_location = edge_location.flip();
  for (size_t i = 0; i < EDGE_LOCATION_ORDER.size(); ++i) {
    if (EDGE_LOCATION_ORDER[i] == edge_location) return {edges[i], false};
    if (EDGE_LOCATION_ORDER[i] == flipped_location) return {edges[i], true};
  }
  throw std::invalid_argument("EdgeLocation not found!");
}

CornerPieceProxy Cube::operator[](const CornerLocation& corner_location) {
  const CornerLocation clockwise_location = corner_location.rotateClockwise();
  const CornerLocation counterclockwise_location =
      corner_location.rotateCounterclockwise();
  for (size_t i = 0; i < CORNER_LOCATION_ORDER.size(); ++i) {
    if (CORNER_LOCATION_ORDER[i] == corner_location)
      return {corners[i], CornerRotationAmount::None};
    // rotate the resulting CornerPieceProxy in the opposite direction
    if (CORNER_LOCATION_ORDER[i] == clockwise_location)
      return {corners[i], CornerRotationAmount::Counterclockwise};
    if (CORNER_LOCATION_ORDER[i] == counterclockwise_location)
      return {corners[i], CornerRotationAmount::Clockwise};
  }
  throw std::invalid_argument("CornerLocation not found!");
}

ConstCornerPieceProxy Cube::operator[](
    const CornerLocation& corner_location) const {
  const CornerLocation clockwise_location = corner_location.rotateClockwise();
  const CornerLocation counterclockwise_location =
      corner_location.rotateCounterclockwise();
  for (size_t i = 0; i < CORNER_LOCATION_ORDER.size(); ++i) {
    if (CORNER_LOCATION_ORDER[i] == corner_location)
      return {corners[i], CornerRotationAmount::None};
    // rotate the resulting ConstCornerPieceProxy in the opposite direction
    if (CORNER_LOCATION_ORDER[i] == clockwise_location)
      return {corners[i], CornerRotationAmount::Counterclockwise};
    if (CORNER_LOCATION_ORDER[i] == counterclockwise_location)
      return {corners[i], CornerRotationAmount::Clockwise};
  }
  throw std::invalid_argument("CornerLocation not found!");
}

void Cube::apply(const Turn& turn) {
  if (turn.rotation_amount == RotationAmount::None) return;

  std::array<EdgeLocation, 4> edge_cycle{};
  std::array<CornerLocation, 4> corner_cycle{};

  using enum Face;
  switch (orientation.apply(turn.face)) {
    case U:
      edge_cycle = {{{U, B}, {U, R}, {U, F}, {U, L}}};
      corner_cycle = {{{U, L, B}, {U, B, R}, {U, R, F}, {U, F, L}}};
      break;
    case F:
      edge_cycle = {{{F, U}, {F, R}, {F, D}, {F, L}}};
      corner_cycle = {{{F, L, U}, {F, U, R}, {F, R, D}, {F, D, L}}};
      break;
    case R:
      edge_cycle = {{{R, U}, {R, B}, {R, D}, {R, F}}};
      corner_cycle = {{{R, F, U}, {R, U, B}, {R, B, D}, {R, D, F}}};
      break;
    case B:
      edge_cycle = {{{B, U}, {B, L}, {B, D}, {B, R}}};
      corner_cycle = {{{B, R, U}, {B, U, L}, {B, L, D}, {B, D, R}}};
      break;
    case L:
      edge_cycle = {{{L, U}, {L, F}, {L, D}, {L, B}}};
      corner_cycle = {{{L, B, U}, {L, U, F}, {L, F, D}, {L, D, B}}};
      break;
    case D:
      edge_cycle = {{{D, F}, {D, R}, {D, B}, {D, L}}};
      corner_cycle = {{{D, L, F}, {D, F, R}, {D, R, B}, {D, B, L}}};
      break;
  }
  switch (turn.rotation_amount) {
    case RotationAmount::Clockwise:
      cycleEdges<4>(
          {edge_cycle[0], edge_cycle[1], edge_cycle[2], edge_cycle[3]});
      cycleCorners<4>(
          {corner_cycle[0], corner_cycle[1], corner_cycle[2], corner_cycle[3]});
      break;
    case RotationAmount::Counterclockwise:
      cycleEdges<4>(
          {edge_cycle[3], edge_cycle[2], edge_cycle[1], edge_cycle[0]});
      cycleCorners<4>(
          {corner_cycle[3], corner_cycle[2], corner_cycle[1], corner_cycle[0]});
      break;
    case RotationAmount::HalfTurn:
      cycleEdges<2>({edge_cycle[0], edge_cycle[2]});
      cycleEdges<2>({edge_cycle[1], edge_cycle[3]});
      cycleCorners<2>({corner_cycle[0], corner_cycle[2]});
      cycleCorners<2>({corner_cycle[1], corner_cycle[3]});
      break;
    case RotationAmount::None:
      throw std::logic_error("turn.rotation_amount became None!");
  }
}

void Cube::apply(const SliceTurn& slice_turn) {
  const auto [turn1, turn2, cube_rotation] = slice_turn.expand();
  apply(turn1);
  apply(turn2);
  apply(cube_rotation);
}

void Cube::apply(const WideTurn& wide_turn) {
  const auto [turn, cube_rotation] = wide_turn.expand();
  apply(turn);
  apply(cube_rotation);
}

void Cube::apply(const CubeRotation& cubeRotation) {
  // don't actually move any pieces, just track the net orientation
  orientation *= cubeRotation;
}

void Cube::apply(const Move& move) {
  if (move.isTurn())
    apply(move.getTurn());
  else if (move.isSliceTurn())
    apply(move.getSliceTurn());
  else if (move.isWideTurn())
    apply(move.getWideTurn());
  else {
    assert(move.isCubeRotation());
    apply(move.getCubeRotation());
  }
}

void Cube::apply(const Algorithm& algorithm) {
  for (const Move& move : algorithm) apply(move);
}

void Cube::scramble() {
  const auto edge_permutation =
      Permutation<EDGE_LOCATION_ORDER.size()>::randomPermutation();
  const auto corner_permutation = [&]() {
    auto corner_perm =
        Permutation<CORNER_LOCATION_ORDER.size()>::randomPermutation();
    if (edge_permutation.isOdd() != corner_perm.isOdd())
      corner_perm.flipParity();
    return corner_perm;
  }();

  edges = edge_permutation.apply(STARTING_EDGE_PIECES);
  corners = corner_permutation.apply(STARTING_CORNER_PIECES);

  bool edge_flip_parity = false;
  for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size() - 1; ++i) {
    if (utility::randomBool()) {
      edges[i].flipInPlace();
      edge_flip_parity ^= true;
    }
  }
  if (edge_flip_parity) edges.back().flipInPlace();

  CornerRotationAmount net_rotation = CornerRotationAmount::None;
  for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size() - 1; ++i) {
    const uint8_t rotation = utility::randomInt<3>();
    if (rotation == 1) {
      corners[i] = corners[i].rotateClockwise();
      net_rotation += CornerRotationAmount::Clockwise;
    }
    else if (rotation == 2) {
      corners[i] = corners[i].rotateCounterclockwise();
      net_rotation += CornerRotationAmount::Counterclockwise;
    }
  }
  corners.back() = corners.back().rotate(-net_rotation);
}

void Cube::setSolved() {
  edges = STARTING_EDGE_PIECES;
  corners = STARTING_CORNER_PIECES;
}

bool Cube::edgesSolved() const { return edges == STARTING_EDGE_PIECES; }

bool Cube::cornersSolved() const { return corners == STARTING_CORNER_PIECES; }

bool Cube::isSolved() const { return edgesSolved() && cornersSolved(); }

bool Cube::isStandardOrientation() const {
  return orientation == CubeOrientation::identity();
}

std::string Cube::toNetString() const {
  //  B
  // LURD
  //  F
  return "    " + toStr(corners[7].second) + toStr(edges[10].second) +
         toStr(corners[6].third) + "\n    " + toStr(edges[6].first) + "B" +
         toStr(edges[5].second) + "\n    " + toStr(corners[0].third) +
         toStr(edges[0].second) + toStr(corners[1].second) + "\n" +
         toStr(corners[7].third) + toStr(edges[6].second) +
         toStr(corners[0].second) + " " + toStr(corners[0].first) +
         toStr(edges[0].first) + toStr(corners[1].first) + " " +
         toStr(corners[1].third) + toStr(edges[5].first) +
         toStr(corners[6].second) + " " + toStr(corners[6].first) +
         toStr(edges[10].first) + toStr(corners[7].first) + "\n" +
         toStr(edges[11].second) + "O" + toStr(edges[3].second) + " " +
         toStr(edges[3].first) + "W" + toStr(edges[1].first) + " " +
         toStr(edges[1].second) + "R" + toStr(edges[9].second) + " " +
         toStr(edges[9].first) + "Y" + toStr(edges[11].first) + "\n" +
         toStr(corners[4].second) + toStr(edges[7].first) +
         toStr(corners[3].third) + " " + toStr(corners[3].first) +
         toStr(edges[2].first) + toStr(corners[2].first) + " " +
         toStr(corners[2].second) + toStr(edges[4].second) +
         toStr(corners[5].third) + " " + toStr(corners[5].first) +
         toStr(edges[8].first) + toStr(corners[4].first) + "\n    " +
         toStr(corners[3].second) + toStr(edges[2].second) +
         toStr(corners[2].third) + "\n    " + toStr(edges[7].second) + "G" +
         toStr(edges[4].first) + "\n    " + toStr(corners[4].third) +
         toStr(edges[8].second) + toStr(corners[5].second);
}
