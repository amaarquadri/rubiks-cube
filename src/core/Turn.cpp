#include "Turn.h"
#include <tuple>

Turn Turn::inv() const {
  return is_slice_turn ? Turn{slice, -rotationAmount}
                       : Turn{face, -rotationAmount};
}

std::string Turn::toStr() const {
  // This syntax tells C++ to use the functions in the global namespace instead
  // of recursion Not sure why this is needed since the functions have different
  // signatures
  return (is_slice_turn ? ::toStr(slice) : ::toStr(face)) +
         ::toStr(rotationAmount);
}

std::pair<size_t, Turn> Turn::parse(const std::string& str) {
  bool is_slice_turn = false;
  auto [consumed, face] = parseFace(str);
  Slice slice;
  if (consumed == 0) {
    // cannot parse a Face, try parsing a Slice instead
    std::tie(consumed, slice) = parseSlice(str);
    if (consumed == 0) {
      // not possible to parse
      return {0, {}};
    }
    is_slice_turn = true;
  }
  std::string remaining = str.substr(consumed, str.size() - consumed);
  const auto [consumed_for_rotation_amount, rotation_amount] =
      parseRotationAmount(remaining);
  return {consumed + consumed_for_rotation_amount,
          is_slice_turn ? Turn{slice, rotation_amount}
                        : Turn{face, rotation_amount}};
}
