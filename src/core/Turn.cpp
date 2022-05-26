#include "Turn.h"
#include <tuple>

Turn Turn::inv() const {
  return is_slice_turn ? Turn{slice, -rotation_amount}
                       : Turn{face, -rotation_amount};
}

std::string Turn::toStr() const {
  // This syntax tells C++ to use the functions in the global namespace instead
  // of recursion Not sure why this is needed since the functions have different
  // signatures
  return (is_slice_turn ? ::toStr(slice) : ::toStr(face)) +
         ::toStr(rotation_amount);
}

std::pair<size_t, Turn> Turn::parse(const std::string& str) {
  // cannot use structured bindings since that will disallow capturing it by
  // reference in the lambda due to a c++17 standards bug
  size_t consumed;
  Face face;
  std::tie(consumed, face) = parseFace(str);

  const auto parse_rotation_amount = [&]() {
    const std::string remaining = str.substr(consumed, str.size() - consumed);
    const auto [consumed_for_rotation_amount, rotation_amount] =
        parseRotationAmount(remaining);
    consumed += consumed_for_rotation_amount;
    return rotation_amount;
  };

  if (consumed != 0) {
    const RotationAmount rotation_amount = parse_rotation_amount();
    return {consumed, Turn{face, rotation_amount}};
  }

  // cannot parse a Face, try parsing a wide Face instead
  std::tie(consumed, face) = parseWideFace(str);
  if (consumed != 0) {
    const RotationAmount rotation_amount = parse_rotation_amount();
    return {consumed, Turn::makeWide(face, rotation_amount)};
  }

  // cannot parse a wide Face, try parsing a Slice instead
  Slice slice;
  std::tie(consumed, slice) = parseSlice(str);
  if (consumed != 0) {
    const RotationAmount rotation_amount = parse_rotation_amount();
    return {consumed, Turn{slice, rotation_amount}};
  }

  return {0, {}};  // not possible to parse
}
