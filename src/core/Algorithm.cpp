#include "Algorithm.h"
#include "Cube.h"
#include "CubeOrientation.h"
#include "Face.h"
#include <algorithm>
#include <tuple>

std::string Algorithm::toStr() const {
  std::string result;
  for (const auto& move : *this) result += move.toStr() + " ";
  return result;
}

bool Algorithm::operator==(const Algorithm& other) const {
  Cube test_cube{*this};
  test_cube.apply(other.inv());
  return test_cube.isSolved() && test_cube.isStandardOrientation();
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
        original_second_last_turn.rotationAmount +
        original_last_turn.rotationAmount;
    if (new_rotation_amount == RotationAmount::None) {
      // remove both moves from moves and previousTurns
      moves.erase(moves.begin() + last_pair.first);
      moves.erase(moves.begin() + second_last_pair.first);
      previousTurns.erase(previousTurns.end() - 1);
      previousTurns.erase(previousTurns.end() - 1);
      return 2;
    } else {
      // update moves
      moves[second_last_pair.first].turn.rotationAmount = new_rotation_amount;
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
  int total_consumed = 0;
  Algorithm moves;
  while (total_consumed < alg.size()) {
    total_consumed += consumeSeparators(
        alg.substr(total_consumed, alg.size() - total_consumed));
    auto [consumed_for_turn, turn] =
        Turn::parse(alg.substr(total_consumed, alg.size() - total_consumed));
    if (consumed_for_turn == 0) {
      // couldn't parse a Turn, try parsing a CubeRotation instead
      auto [consumed_for_cube_rotation, cubeRotation] = CubeRotation::parse(
          alg.substr(total_consumed, alg.size() - total_consumed));
      if (consumed_for_cube_rotation == 0) break;
      moves.emplace_back(cubeRotation);
      total_consumed += consumed_for_cube_rotation;
    } else {
      moves.emplace_back(turn);
      total_consumed += consumed_for_turn;
    }
  }
  return moves;
}

/**
 * @return Tuple of (number of characters consumed, whether the rotation amount
 * is clockwise, number of rotations)
 */
static std::tuple<size_t, bool, size_t> parseExpandedRotationAmount(
    const std::string& str) {
  bool clockwise = true;
  unsigned int rotation_amount = 0;
  int consumed = 0;
  for (char chr : str) {
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
 * @return Tuple of (number of characters consumed, the turn, the number of
 * times the turn should be repeated)
 */
static std::tuple<size_t, Turn, size_t> parseExpandedTurns(
    const std::string& str) {
  size_t consumed;
  Turn turn{};
  std::tie(consumed, turn.face) = parseFace(str);
  if (consumed == 0) {
    // cannot parse a Face, try parsing a Slice instead
    std::tie(consumed, turn.slice) = parseSlice(str);
    if (consumed == 0) {
      // not possible to parse
      return {0, turn, 0};
    }
    turn.is_slice_turn = true;
  }
  const std::string remaining = str.substr(consumed, str.size() - consumed);
  const auto [consumed_for_rotation_amount, clockwise, rotation_amount] =
      parseExpandedRotationAmount(remaining);
  turn.rotationAmount =
      clockwise ? RotationAmount::Clockwise : RotationAmount::Counterclockwise;
  return {consumed + consumed_for_rotation_amount, turn, rotation_amount};
}

Algorithm Algorithm::parseExpanded(const std::string& alg) {
  // TODO: reduce code duplication for the parseExpanded functions
  int total_consumed = 0;
  Algorithm moves;
  while (total_consumed < alg.size()) {
    total_consumed += consumeSeparators(
        alg.substr(total_consumed, alg.size() - total_consumed));
    auto [consumed_for_turn, turn, rotation_amount] = parseExpandedTurns(
        alg.substr(total_consumed, alg.size() - total_consumed));
    if (consumed_for_turn == 0) {
      // couldn't parse a Turn, try parsing a CubeRotation instead
      auto [consumed_for_cube_rotation, cubeRotation] = CubeRotation::parse(
          alg.substr(total_consumed, alg.size() - total_consumed));
      if (consumed_for_cube_rotation == 0) break;
      moves.emplace_back(cubeRotation);
      total_consumed += consumed_for_cube_rotation;
    } else {
      for (int i = 0; i < rotation_amount; i++) moves.emplace_back(turn);
      total_consumed += consumed_for_turn;
    }
  }
  return moves;
}

std::pair<Algorithm, Algorithm> Algorithm::parseScrambleSolve(
    const std::string& alg) {
  size_t scramble_length = 0;
  while (scramble_length < alg.size() && alg[scramble_length] != '\n')
    scramble_length++;

  if (scramble_length == alg.size())
    throw std::invalid_argument(
        "No new line characters! Cannot distinguish scramble from solve");

  // use parseExpanded for the attempted solution so that the division between
  // blindsolving moves can easily be determined
  return {parse(alg.substr(0, scramble_length)),
          parseExpanded(alg.substr(scramble_length + 1,
                                   alg.size() - scramble_length - 1))};
}

Algorithm Algorithm::inv() const {
  Algorithm inverse_moves{size()};
  std::transform(rbegin(), rend(), inverse_moves.begin(),
                 [](const Move& move) { return move.inv(); });
  return inverse_moves;
}

Algorithm Algorithm::subAlgorithm(const size_t& start,
                                  const size_t& end) const {
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
