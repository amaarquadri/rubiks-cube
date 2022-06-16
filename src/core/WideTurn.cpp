#include "WideTurn.h"
#include "Face.h"
#include "RotationAmount.h"
#include <cstddef>
#include <string>
#include <utility>

std::string WideTurn::toStr() const {
  // This syntax tells C++ to use the functions in the global namespace instead
  // of recursion Not sure why this is needed since the functions have different
  // signatures
  return ::toStrLower(face) + ::toStr(rotation_amount);
}

std::pair<Turn, CubeRotation> WideTurn::expand() const {
  const auto [rotation_axis, reverse] = getRotationAxis(face);
  return {Turn{getOpposite(face), rotation_amount},
          CubeRotation{rotation_axis,
                       reverse ? -rotation_amount : rotation_amount}};
}

std::pair<size_t, WideTurn> WideTurn::parse(const std::string& str) {
  const auto [consumed, face] = parseWideFace(str);
  if (consumed == 0) return {0, {}};  // not possible to parse
  const std::string remaining = str.substr(consumed, str.size() - consumed);
  const auto [consumed_for_rotation_amount, rotation_amount] =
      parseRotationAmount(remaining);
  return {consumed + consumed_for_rotation_amount,
          WideTurn{face, rotation_amount}};
}
