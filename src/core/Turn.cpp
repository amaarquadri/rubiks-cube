#include "Turn.h"
#include "Face.h"
#include "RotationAmount.h"
#include <cstddef>
#include <string>
#include <utility>

std::string Turn::toStr() const {
  // This syntax tells C++ to use the functions in the global namespace instead
  // of recursion. Not sure why this is needed since the functions have
  // different signatures
  return ::toStr(face) + ::toStr(rotation_amount);
}

std::pair<size_t, Turn> Turn::parse(const std::string& str) {
  const auto [consumed, face] = parseFace(str);
  if (consumed == 0) return {0, {}};  // not possible to parse
  const std::string remaining = str.substr(consumed, str.size() - consumed);
  const auto [consumed_for_rotation_amount, rotation_amount] =
      parseRotationAmount(remaining);
  return {consumed + consumed_for_rotation_amount, Turn{face, rotation_amount}};
}
