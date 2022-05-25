#include "Move.h"

Move::Move(const Move& other) { *this = other; }

Move& Move::operator=(const Move& other) {
  isTurn = other.isTurn;
  if (isTurn)
    turn = other.turn;
  else
    cubeRotation = other.cubeRotation;
  return *this;
}

Move Move::inv() const {
  if (isTurn) {
    return Move{turn.inv()};
  } else {
    return Move{cubeRotation.inv()};
  }
}

std::string Move::toStr() const {
  return isTurn ? turn.toStr() : cubeRotation.toStr();
}

std::pair<size_t, Move> Move::parse(const std::string& str) {
  const auto [consumed_for_turn, turn] = Turn::parse(str);
  if (consumed_for_turn != 0) return {consumed_for_turn, Move{turn}};

  // couldn't parse a Turn, try parsing a CubeRotation instead
  const auto [consumed_for_cube_rotation, cube_rotation] =
      CubeRotation::parse(str);
  if (consumed_for_cube_rotation != 0)
    return {consumed_for_cube_rotation, Move{cube_rotation}};

  return {0, {}};  // couldn't parse a Move
}
