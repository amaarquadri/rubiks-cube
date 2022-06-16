#include "Move.h"
#include "CubeRotation.h"
#include "SliceTurn.h"
#include "Turn.h"
#include "WideTurn.h"
#include <cassert>
#include <string>
#include <variant>

bool Move::isTurn() const { return std::holds_alternative<Turn>(*this); }

bool Move::isSliceTurn() const {
  return std::holds_alternative<SliceTurn>(*this);
}

bool Move::isWideTurn() const {
  return std::holds_alternative<WideTurn>(*this);
}

bool Move::isCubeRotation() const {
  return std::holds_alternative<CubeRotation>(*this);
}

const Turn& Move::getTurn() const {
  assert(isTurn());
  return std::get<Turn>(*this);
}

Turn& Move::getTurn() {
  assert(isTurn());
  return std::get<Turn>(*this);
}

const SliceTurn& Move::getSliceTurn() const {
  assert(isSliceTurn());
  return std::get<SliceTurn>(*this);
}

SliceTurn& Move::getSliceTurn() {
  assert(isSliceTurn());
  return std::get<SliceTurn>(*this);
}

const WideTurn& Move::getWideTurn() const {
  assert(isWideTurn());
  return std::get<WideTurn>(*this);
}

WideTurn& Move::getWideTurn() {
  assert(isWideTurn());
  return std::get<WideTurn>(*this);
}

const CubeRotation& Move::getCubeRotation() const {
  assert(isCubeRotation());
  return std::get<CubeRotation>(*this);
}

CubeRotation& Move::getCubeRotation() {
  assert(isCubeRotation());
  return std::get<CubeRotation>(*this);
}

Move Move::inv() const {
  if (isTurn())
    return Move{getTurn().inv()};
  else if (isSliceTurn())
    return Move{getSliceTurn().inv()};
  else if (isWideTurn())
    return Move{getWideTurn().inv()};
  else {
    assert(isCubeRotation());
    return Move{getCubeRotation().inv()};
  }
}

std::string Move::toStr() const {
  if (isTurn())
    return getTurn().toStr();
  else if (isSliceTurn())
    return getSliceTurn().toStr();
  else if (isWideTurn())
    return getWideTurn().toStr();
  else {
    assert(isCubeRotation());
    return getCubeRotation().toStr();
  }
}

std::pair<size_t, Move> Move::parse(const std::string& str) {
  // try parsing a Turn
  const auto [consumed_for_turn, turn] = Turn::parse(str);
  if (consumed_for_turn != 0) return {consumed_for_turn, Move{turn}};

  // couldn't parse a Turn, try parsing a SliceTurn instead
  const auto [consumed_for_slice_turn, slice_turn] = SliceTurn::parse(str);
  if (consumed_for_slice_turn != 0)
    return {consumed_for_slice_turn, Move{slice_turn}};

  // couldn't parse a SliceTurn, try parsing a WideTurn instead
  const auto [consumed_for_wide_turn, wide_turn] = WideTurn::parse(str);
  if (consumed_for_wide_turn != 0)
    return {consumed_for_wide_turn, Move{wide_turn}};

  // couldn't parse a WideTurn, try parsing a CubeRotation instead
  const auto [consumed_for_cube_rotation, cube_rotation] =
      CubeRotation::parse(str);
  if (consumed_for_cube_rotation != 0)
    return {consumed_for_cube_rotation, Move{cube_rotation}};

  return {0, {}};  // couldn't parse a Move
}
