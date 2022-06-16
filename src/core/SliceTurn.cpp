#include "SliceTurn.h"
#include "RotationAmount.h"
#include "Slice.h"
#include <cstddef>
#include <string>
#include <tuple>
#include <utility>

std::string SliceTurn::toStr() const {
  // This syntax tells C++ to use the functions in the global namespace instead
  // of recursion. Not sure why this is needed since the functions have
  // different signatures
  return ::toStr(slice) + ::toStr(rotation_amount);
}

std::tuple<Turn, Turn, CubeRotation> SliceTurn::expand() const {
  const Face rotation_face = getRotationFace(slice);
  const auto [rotation_axis, reverse] = getRotationAxis(slice);
  return {Turn{rotation_face, -rotation_amount},
          Turn{getOpposite(rotation_face), rotation_amount},
          CubeRotation{rotation_axis,
                       reverse ? -rotation_amount : rotation_amount}};
}

std::pair<size_t, SliceTurn> SliceTurn::parse(const std::string& str) {
  const auto [consumed, slice] = parseSlice(str);
  if (consumed == 0) return {0, {}};  // not possible to parse
  const std::string remaining = str.substr(consumed, str.size() - consumed);
  const auto [consumed_for_rotation_amount, rotation_amount] =
      parseRotationAmount(remaining);
  return {consumed + consumed_for_rotation_amount,
          SliceTurn{slice, rotation_amount}};
}
