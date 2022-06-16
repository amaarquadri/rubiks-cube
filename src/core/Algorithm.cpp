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

Algorithm Algorithm::random(const size_t& size) {
  Algorithm alg{};
  alg.reserve(size);
  while (alg.size() < size) {
    for (size_t i = 0; i < size - alg.size(); i++)
      alg.push_back(Move{Turn::random()});
    alg.cancelMoves();
  }
  return alg;
}

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

static int mergeTurns(
    Algorithm& moves,
    std::vector<std::pair<int, CubeOrientation>>& previousTurns) {
  // merges any turns resulting from the last element in previousTurns, and
  // updates both vectors returns the number of moves that were cancelled
  if (previousTurns.size() < 2) return 0;

  std::pair<int, CubeOrientation> last_pair =
      previousTurns[previousTurns.size() - 1];
  std::pair<int, CubeOrientation> second_last_pair =
      previousTurns[previousTurns.size() - 2];
  Turn original_last_turn = last_pair.second.apply(
      moves[last_pair.first]
          .turn);  // equivalent last turn from original CubeRotation
  Turn original_second_last_turn = second_last_pair.second.apply(
      moves[second_last_pair.first]
          .turn);  // equivalent second last turn from original CubeRotation

  if (original_last_turn.face == original_second_last_turn.face) {
    RotationAmount new_rotation_amount =
        original_second_last_turn.rotation_amount +
        original_last_turn.rotation_amount;
    if (new_rotation_amount == RotationAmount::None) {
      // remove both moves from moves and previousTurns
      moves.erase(moves.begin() + last_pair.first);
      moves.erase(moves.begin() + second_last_pair.first);
      previousTurns.erase(previousTurns.end() - 1);
      previousTurns.erase(previousTurns.end() - 1);
      return 2;
    } else {
      // update moves
      moves[second_last_pair.first].turn.rotation_amount = new_rotation_amount;
      // remove second move from moves and previousTurns
      moves.erase(moves.begin() + last_pair.first);
      previousTurns.erase(previousTurns.end() - 1);
      return 1;
    }
  } else if (getOpposite(original_last_turn.face) ==
                 original_second_last_turn.face &&
             previousTurns.size() >= 3) {
    std::pair<int, CubeOrientation> third_last_pair =
        previousTurns[previousTurns.size() - 3];
    Turn original_third_last_turn = third_last_pair.second.apply(
        moves[third_last_pair.first]
            .turn);  // equivalent third last turn from original CubeRotation
    if (original_last_turn.face == original_third_last_turn.face) {
      // temporarily remove second_last_pair, recurse to combine the last and
      // 3rd last pairs, then add it back
      previousTurns.erase(previousTurns.end() - 2);
      int merged_turns = mergeTurns(moves, previousTurns);
      previousTurns.push_back(second_last_pair);
      return merged_turns;
    }
  }
  return 0;
}

void Algorithm::cancelMoves() {
  if (empty()) return;

  // first pass, combine any adjacent CubeRotations with the same RotationAxis
  for (auto it = begin() + 1; it < end();) {
    if (!(it - 1)->isTurn && !it->isTurn &&
        (it - 1)->cubeRotation.rotationAxis == it->cubeRotation.rotationAxis) {
      (it - 1)->cubeRotation.rotationAmount =
          (it - 1)->cubeRotation.rotationAmount +
          it->cubeRotation.rotationAmount;
      it = erase(it);
    } else
      it++;
  }

  std::vector<std::pair<int, CubeOrientation>>
      previousTurns;  // index, CubeOrientation at index
  CubeOrientation orientation = CubeOrientation::identity();
  for (int index = 0; index < size(); index++) {
    Move move = (*this)[index];
    if (move.isTurn) {
      previousTurns.emplace_back(index, orientation);
      index -= mergeTurns(*this, previousTurns);
    } else {
      orientation *= move.cubeRotation;
    }
  }
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
  std::tie(consumed, cube_rotation.rotationAxis) = parseRotationAxis(str);
  if (consumed != 0) {
    const size_t iterations = finisher(cube_rotation.rotationAmount);
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
