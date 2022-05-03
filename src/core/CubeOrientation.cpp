#include "CubeOrientation.h"
#include <stdexcept>

bool CubeOrientation::operator==(const CubeOrientation& other) const {
  return top == other.top && front == other.front;
}

Face CubeOrientation::getRightFace() const { return getRight(top, front); }

Face CubeOrientation::getBackFace() const { return getOpposite(front); }

Face CubeOrientation::getLeftFace() const { return getLeft(top, front); }

Face CubeOrientation::getBottomFace() const { return getOpposite(top); }

CubeOrientation CubeOrientation::identity() { return {U, F}; }

CubeOrientation CubeOrientation::inv() const {
  Face inv_top, inv_front;
  for (Face face : {U, F, R, B, L, D}) {
    if (apply(face) == U)
      inv_top = face;
    else if (apply(face) == F)
      inv_front = face;
  }
  return CubeOrientation{inv_top, inv_front};
}

CubeOrientation CubeOrientation::operator*(
    const CubeRotation& cubeRotation) const {
  CubeOrientation product{top, front};
  for (int i = 0; i < static_cast<uint8_t>(cubeRotation.rotationAmount); i++) {
    switch (cubeRotation.rotationAxis) {
      case X: {
        Face new_top = product.front;
        Face new_front = product.getBottomFace();
        product.top = new_top;
        product.front = new_front;
        break;
      }
      case Y:
        product.front = product.getRightFace();
        break;
      case Z:
        product.top = product.getLeftFace();
        break;
    }
  }
  return product;
}

void CubeOrientation::operator*=(const CubeRotation& cubeRotation) {
  CubeOrientation product = (*this) * cubeRotation;
  top = product.top;
  front = product.front;
}

Face CubeOrientation::apply(const Face& face) const {
  switch (face) {
    case U:
      return top;
    case F:
      return front;
    case R:
      return getRightFace();
    case B:
      return getBackFace();
    case L:
      return getLeftFace();
    case D:
      return getBottomFace();
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<Slice, bool> CubeOrientation::apply(const Slice& slice) const {
  return fromRotationFace(apply(getRotationFace(slice)));
}

Turn CubeOrientation::apply(const Turn& turn) const {
  if (turn.is_slice_turn) {
    auto [new_slice, reversed] = apply(turn.slice);
    return Turn{new_slice,
                reversed ? ::inv(turn.rotationAmount) : turn.rotationAmount};
  } else {
    return Turn{apply(turn.face), turn.rotationAmount};
  }
}

CubeRotation CubeOrientation::apply(const CubeRotation& cubeRotation) const {
  // TODO: what is this lol
  return CubeRotation();
}
