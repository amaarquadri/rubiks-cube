#include "Algorithm.h"
#include "Cube.h"
#include "CubeOrientation.h"
#include "Face.h"
#include "Move.h"
#include "SliceTurn.h"
#include "Turn.h"
#include "WideTurn.h"
#include <cassert>
#include <tuple>

std::string Algorithm::toStr() const {
  std::string result;
  for (const auto& move : *this) result += move.toStr() + " ";
  return result;
}

bool Algorithm::operator==(const Algorithm& other) const {
  const Cube test_cube{(*this) + other.inv()};
  return test_cube.isSolved() && test_cube.isStandardOrientation();
}

bool Algorithm::operator!=(const Algorithm& other) const {
  return !((*this) == other);
}

void Algorithm::cancelMoves() {
  if (empty()) return;
  toStandardForm();

  // use i + 1 < size() instead of i < size() - 1 to avoid underflow
  for (size_t i = 0; i + 1 < size();) {
    Move& move = (*this)[i];
    if (move.isCubeRotation()) {
      ++i;
      continue;
    }
    Move& next_move = (*this)[i + 1];

    if (move.getTurn().face == next_move.getTurn().face) {
      move.getTurn().rotation_amount += next_move.getTurn().rotation_amount;
      erase(begin() + (i + 1));
      if (move.getTurn().rotation_amount == RotationAmount::None) {
        erase(begin() + i);
        if (i > 0) --i;
      }
    } else
      ++i;
  }
}

void Algorithm::toStandardForm() {
  // replace SliceTurns and WideTurns with their expansions
  for (size_t i = 0; i < size(); ++i) {
    Move& move = (*this)[i];
    if (move.isSliceTurn()) {
      const auto [turn1, turn2, cube_rotation] = move.getSliceTurn().expand();
      move = Move{turn1};
      insert(begin() + (i + 1), Move{turn2});
      insert(begin() + (i + 2), Move{cube_rotation});
      i += 2;  // skip next 2 elements since they are known not to be SliceTurns
               // or WideTurns
    } else if (move.isWideTurn()) {
      const auto [turn, cube_rotation] = move.getWideTurn().expand();
      move = Move{turn};
      insert(begin() + (i + 1), Move{cube_rotation});
      ++i;  // skip next element since it is known not to be a SliceTurn or a
            // WideTurn
    }
  }

  CubeOrientation cube_orientation = CubeOrientation::identity();
  for (size_t i = size(); i-- > 0;) {  // iterate from i - 1 to 0 inclusive
    Move& move = (*this)[i];
    if (move.isCubeRotation()) {
      cube_orientation *= move.getCubeRotation().inv();
      erase(begin() + i);
    } else {
      assert(move.isTurn());
      move = cube_orientation.apply(move);
    }
  }
  const auto [first, second] = cube_orientation.solve();
  if (first) {
    insert(begin(), Move{first.value()});
    if (second) insert(begin() + 1, Move{second.value()});
  }
}

bool Algorithm::isStandardForm() const {
  if (empty()) return true;
  if (!front().isTurn() && !front().isCubeRotation()) return false;
  if (size() == 1) return true;
  if (!(*this)[1].isTurn() && !(*this)[1].isCubeRotation()) return false;
  if (front().isTurn() && (*this)[1].isCubeRotation()) return false;
  for (size_t i = 2; i < size(); ++i)
    if (!(*this)[i].isTurn()) return false;
  return true;
}

static size_t consumeSeparators(const std::string& alg) {
  size_t consumed = 0;
  while (consumed < alg.size()) {
    switch (alg[consumed]) {
      case ' ':
      case '\n':
        consumed++;
        break;
      case '/':
        while (consumed != alg.size() && alg[consumed] != '\n') consumed++;
        if (consumed != alg.size())
          consumed++;  // consume the new line character too, if we found it
        break;
      default:
        return consumed;
    }
  }
  return consumed;  // consumed entire alg
}

Algorithm Algorithm::parse(const std::string& alg) {
  size_t total_consumed = 0;
  const auto get_remaining_alg = [&]() {
    return alg.substr(total_consumed, alg.size() - total_consumed);
  };
  Algorithm moves{};
  while (total_consumed < alg.size()) {
    total_consumed += consumeSeparators(get_remaining_alg());
    const auto [consumed_for_move, move] = Move::parse(get_remaining_alg());
    if (consumed_for_move != 0) {
      moves.push_back(move);
      total_consumed += consumed_for_move;
    } else
      break;
  }
  return moves;
}

/**
 * @return Tuple of (number of characters consumed, whether the rotation amount
 * is clockwise, number of rotations)
 */
static std::tuple<size_t, bool, size_t> parseExpandedRotationAmount(
    const std::string& str) {
  size_t consumed = 0;
  bool clockwise = true;
  size_t rotation_amount = 0;
  for (const char& chr : str) {
    if (chr >= '0' && chr <= '9') {
      const size_t digit = chr - '0';
      rotation_amount = 10 * rotation_amount + digit;
      consumed++;
    } else if (chr == '\'') {
      clockwise = false;
      consumed++;
      break;
    } else
      break;
  }
  return {consumed, clockwise, rotation_amount == 0 ? 1 : rotation_amount};
}

/**
 * @return Tuple of (number of characters consumed, the Move, the number of
 * times the Move should be repeated)
 */
static std::tuple<size_t, Move, size_t> parseExpandedMove(
    const std::string& str) {
  size_t consumed;

  const auto finisher = [&](RotationAmount& rotation_amount) {
    const std::string remaining = str.substr(consumed, str.size() - consumed);
    const auto [consumed_for_rotation_amount, clockwise, iterations] =
        parseExpandedRotationAmount(remaining);
    consumed += consumed_for_rotation_amount;
    rotation_amount = clockwise ? RotationAmount::Clockwise
                                : RotationAmount::Counterclockwise;
    return iterations;
  };

  Turn turn;
  std::tie(consumed, turn.face) = parseFace(str);
  if (consumed != 0) {
    const size_t iterations = finisher(turn.rotation_amount);
    return {consumed, Move{turn}, iterations};
  }

  // cannot parse a Face, try parsing a Slice instead
  SliceTurn slice_turn;
  std::tie(consumed, slice_turn.slice) = parseSlice(str);
  if (consumed != 0) {
    const size_t iterations = finisher(slice_turn.rotation_amount);
    return {consumed, Move{slice_turn}, iterations};
  }

  // cannot parse a Slice, try parsing a wide Face instead
  WideTurn wide_turn;
  std::tie(consumed, wide_turn.face) = parseWideFace(str);
  if (consumed != 0) {
    const size_t iterations = finisher(wide_turn.rotation_amount);
    return {consumed, Move{wide_turn}, iterations};
  }

  // cannot parse a wide Face, try parsing a RotationAxis instead
  CubeRotation cube_rotation;
  std::tie(consumed, cube_rotation.rotation_axis) = parseRotationAxis(str);
  if (consumed != 0) {
    const size_t iterations = finisher(cube_rotation.rotation_amount);
    return {consumed, Move{cube_rotation}, iterations};
  }

  return {0, {}, 0};  // not possible to parse
}

Algorithm Algorithm::parseExpanded(const std::string& alg) {
  size_t total_consumed = 0;
  const auto get_remaining_alg = [&]() {
    return alg.substr(total_consumed, alg.size() - total_consumed);
  };
  Algorithm moves;
  while (total_consumed < alg.size()) {
    total_consumed += consumeSeparators(get_remaining_alg());
    const auto [consumed_for_move, move, iterations] =
        parseExpandedMove(get_remaining_alg());
    if (consumed_for_move == 0) break;
    for (size_t i = 0; i < iterations; i++) moves.push_back(move);
    total_consumed += consumed_for_move;
  }
  return moves;
}

Algorithm Algorithm::inv() const {
  Algorithm inverse_moves{size()};
  std::transform(rbegin(), rend(), inverse_moves.begin(),
                 [](const Move& move) { return move.inv(); });
  return inverse_moves;
}

Algorithm Algorithm::subAlgorithm(const size_t& start,
                                  const size_t& end) const {
  assert(start <= end);
  assert(end <= size());
  Algorithm sub_moves{end - start};
  for (size_t i = 0; i < sub_moves.size(); i++)
    sub_moves[i] = (*this)[start + i];
  return sub_moves;
}

Algorithm Algorithm::operator+(const Algorithm& other) const {
  Algorithm sum_moves{size() + other.size()};
  auto it = std::copy(begin(), end(), sum_moves.begin());
  std::copy(other.begin(), other.end(), it);
  return sum_moves;
}

Algorithm Algorithm::operator*(const size_t& times) const {
  Algorithm repeated_moves{size() * times};
  auto it = repeated_moves.begin();
  for (size_t i = 0; i < times; i++) it = std::copy(begin(), end(), it);
  return repeated_moves;
}

Algorithm Algorithm::withSetup(const std::string& setup_alg_string) const {
  const Algorithm setup_alg = Algorithm::parse(setup_alg_string);
  return setup_alg + (*this) + setup_alg.inv();
}

Algorithm Algorithm::commutator(const std::string& first_alg_str,
                                const std::string& second_alg_str) {
  const Algorithm first_alg = Algorithm::parse(first_alg_str);
  const Algorithm second_alg = Algorithm::parse(second_alg_str);
  return first_alg + second_alg + first_alg.inv() + second_alg.inv();
}
