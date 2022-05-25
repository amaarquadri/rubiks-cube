#include "CubeRotation.h"

CubeRotation CubeRotation::inv() const {
  return {rotationAxis, -rotationAmount};
}

std::string CubeRotation::toStr() const {
  return ::toStr(rotationAxis) + ::toStr(rotationAmount);
}

std::pair<int, CubeRotation> CubeRotation::parse(const std::string& str) {
  auto [consumed_for_rotation_axis, rotationAxis] = parseRotationAxis(str);
  if (consumed_for_rotation_axis == 0) return {0, {}};  // not possible to parse
  std::string remaining = str.substr(consumed_for_rotation_axis,
                                     str.size() - consumed_for_rotation_axis);
  auto [consumed_for_rotation_amount, rotation_amount] =
      parseRotationAmount(remaining);
  return {consumed_for_rotation_axis + consumed_for_rotation_amount,
          {rotationAxis, rotation_amount}};
}
