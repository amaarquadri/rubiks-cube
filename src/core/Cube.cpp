#include "Cube.h"
#include "Permutation.h"
#include <array>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>

static unsigned seed =  // NOLINT(cert-err58-cpp)
    std::chrono::system_clock::now().time_since_epoch().count();
static auto random_engine =  // NOLINT(cert-err58-cpp)
    std::default_random_engine(seed);
static std::uniform_int_distribution<uint8_t>
    bool_distribution  // NOLINT(cert-err58-cpp)
    (0, 1);
static std::uniform_int_distribution<uint8_t>
    three_distribution  // NOLINT(cert-err58-cpp)
    (0, 2);

EdgePieceProxy Cube::operator[](const EdgeLocation& edge_location) {
  const EdgeLocation flipped_location = edge_location.flip();
  for (size_t i = 0; i < 12; i++) {
    if (EDGE_LOCATION_ORDER[i] == edge_location) return {edges[i], false};
    if (EDGE_LOCATION_ORDER[i] == flipped_location) return {edges[i], true};
  }
  throw std::invalid_argument("EdgeLocation not found!");
}

ConstEdgePieceProxy Cube::operator[](const EdgeLocation& edge_location) const {
  const EdgeLocation flipped_location = edge_location.flip();
  for (size_t i = 0; i < 12; i++) {
    if (EDGE_LOCATION_ORDER[i] == edge_location) return {edges[i], false};
    if (EDGE_LOCATION_ORDER[i] == flipped_location) return {edges[i], true};
  }
  throw std::invalid_argument("EdgeLocation not found!");
}

CornerPieceProxy Cube::operator[](const CornerLocation& corner_location) {
  const CornerLocation clockwise_location = corner_location.rotateClockwise();
  const CornerLocation counterclockwise_location =
      corner_location.rotateCounterClockwise();
  for (size_t i = 0; i < 8; i++) {
    if (CORNER_LOCATION_ORDER[i] == corner_location)
      return {corners[i], CornerRotationAmount::NONE};
    if (CORNER_LOCATION_ORDER[i] == clockwise_location)
      return {corners[i], CornerRotationAmount::CLOCKWISE};
    if (CORNER_LOCATION_ORDER[i] == counterclockwise_location)
      return {corners[i], CornerRotationAmount::COUNTERCLOCKWISE};
  }
  throw std::invalid_argument("CornerLocation not found!");
}

ConstCornerPieceProxy Cube::operator[](
    const CornerLocation& corner_location) const {
  const CornerLocation clockwise_location = corner_location.rotateClockwise();
  const CornerLocation counterclockwise_location =
      corner_location.rotateCounterClockwise();
  for (size_t i = 0; i < 8; i++) {
    if (CORNER_LOCATION_ORDER[i] == corner_location)
      return {corners[i], CornerRotationAmount::NONE};
    if (CORNER_LOCATION_ORDER[i] == clockwise_location)
      return {corners[i], CornerRotationAmount::CLOCKWISE};
    if (CORNER_LOCATION_ORDER[i] == counterclockwise_location)
      return {corners[i], CornerRotationAmount::COUNTERCLOCKWISE};
  }
  throw std::invalid_argument("CornerLocation not found!");
}

void Cube::apply(const Turn& turn) {
  if (turn.rotationAmount == RotationAmount::NONE) return;

  if (turn.is_slice_turn) {
    apply(Turn{getRotationFace(turn.slice), inv(turn.rotationAmount)});
    apply(Turn{getOpposite(getRotationFace(turn.slice)), turn.rotationAmount});
    auto [rotationAxis, reverse] = getRotationAxis(turn.slice);
    apply(CubeRotation{rotationAxis, reverse ? inv(turn.rotationAmount)
                                             : turn.rotationAmount});
    return;
  }

  std::array<EdgeLocation, 4> edgeCycle{};
  std::array<CornerLocation, 4> cornerCycle{};

  using Face::U, Face::F, Face::R, Face::B, Face::L, Face::D;
  switch (orientation.apply(turn.face)) {
    case U:
      edgeCycle = {{{U, B}, {U, R}, {U, F}, {U, L}}};
      cornerCycle = {{{U, L, B}, {U, B, R}, {U, R, F}, {U, F, L}}};
      break;
    case F:
      edgeCycle = {{{F, U}, {F, R}, {F, D}, {F, L}}};
      cornerCycle = {{{F, L, U}, {F, U, R}, {F, R, D}, {F, D, L}}};
      break;
    case R:
      edgeCycle = {{{R, U}, {R, B}, {R, D}, {R, F}}};
      cornerCycle = {{{R, F, U}, {R, U, B}, {R, B, D}, {R, D, F}}};
      break;
    case B:
      edgeCycle = {{{B, U}, {B, L}, {B, D}, {B, R}}};
      cornerCycle = {{{B, R, U}, {B, U, L}, {B, L, D}, {B, D, R}}};
      break;
    case L:
      edgeCycle = {{{L, U}, {L, F}, {L, D}, {L, B}}};
      cornerCycle = {{{L, B, U}, {L, U, F}, {L, F, D}, {L, D, B}}};
      break;
    case D:
      edgeCycle = {{{D, F}, {D, R}, {D, B}, {D, L}}};
      cornerCycle = {{{D, L, F}, {D, F, R}, {D, R, B}, {D, B, L}}};
      break;
  }
  switch (turn.rotationAmount) {
    case RotationAmount::CLOCKWISE:
      cycleEdges<4>({edgeCycle[0], edgeCycle[1], edgeCycle[2], edgeCycle[3]});
      cycleCorners<4>(
          {cornerCycle[0], cornerCycle[1], cornerCycle[2], cornerCycle[3]});
      break;
    case RotationAmount::COUNTERCLOCKWISE:
      cycleEdges<4>({edgeCycle[0], edgeCycle[3], edgeCycle[2], edgeCycle[1]});
      cycleCorners<4>(
          {cornerCycle[0], cornerCycle[3], cornerCycle[2], cornerCycle[1]});
      break;
    case RotationAmount::HALF_TURN:
      cycleEdges<2>({edgeCycle[0], edgeCycle[2]});
      cycleEdges<2>({edgeCycle[1], edgeCycle[3]});
      cycleCorners<2>({cornerCycle[0], cornerCycle[2]});
      cycleCorners<2>({cornerCycle[1], cornerCycle[3]});
      break;
    case RotationAmount::NONE:
      throw std::logic_error("turn.rotationAmount became NONE!");
  }
}

void Cube::apply(const CubeRotation& cubeRotation) {
  // don't actually move any pieces, just track the net orientation
  orientation *= cubeRotation;
}

void Cube::apply(const Move& move) {
  if (move.isTurn)
    apply(move.turn);
  else
    apply(move.cubeRotation);
}

void Cube::apply(const Algorithm& algorithm) {
  for (const Move& move : algorithm.moves) {
    apply(move);
  }
}

void Cube::scramble() {
  Permutation<12> edge_permutation =
      Permutation<12>::randomPermutation(random_engine);
  Permutation<8> corner_permutation =
      Permutation<8>::randomPermutation(random_engine);
  if (edge_permutation.isOdd() != corner_permutation.isOdd())
    corner_permutation.flipParity();

  edges = edge_permutation.apply(STARTING_EDGE_PIECES);
  corners = corner_permutation.apply(STARTING_CORNER_PIECES);

  bool edgeFlipParity = false;
  for (int i = 0; i < 11; i++) {
    if (bool_distribution(random_engine) == 1) {
      edges[i] = edges[i].flip();
      edgeFlipParity = !edgeFlipParity;
    }
  }
  if (edgeFlipParity) edges[11] = edges[11].flip();

  uint8_t cornerRotationParity = 0;
  for (int i = 0; i < 7; i++) {
    uint8_t rotation = three_distribution(random_engine);
    if (rotation == 1)
      corners[i] = corners[i].rotateClockwise();
    else if (rotation == 2)
      corners[i] = corners[i].rotateCounterclockwise();
    cornerRotationParity += rotation;
  }
  uint8_t lastRotation = cornerRotationParity % 3;
  if (lastRotation == 2)
    corners[7] = corners[7].rotateClockwise();  // need another 1 to get to 3
  else if (lastRotation == 1)
    corners[7] =
        corners[7].rotateCounterclockwise();  // need another 2 to get to 3
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

bool Cube::operator==(const Cube& other) const {
  return edges == other.edges && corners == other.corners &&
         orientation == other.orientation;
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
