#include "CubeOrientation.h"
#include "CubeRotation.h"
#include "Face.h"
#include "Move.h"
#include "RandomUtils.h"
#include "Slice.h"
#include "SliceTurn.h"
#include "Turn.h"
#include "WideTurn.h"
#include <cassert>
#include <cstddef>
#include <optional>
#include <stdexcept>

bool CubeOrientation::isValid() const {
  return top != front && top != getOpposite(front);
}

bool CubeOrientation::operator==(const CubeOrientation& other) const {
  return top == other.top && front == other.front;
}

Face CubeOrientation::getRightFace() const { return getRight(top, front); }

Face CubeOrientation::getBackFace() const { return getOpposite(front); }

Face CubeOrientation::getLeftFace() const { return getLeft(top, front); }

Face CubeOrientation::getBottomFace() const { return getOpposite(top); }

CubeOrientation CubeOrientation::random() {
  const uint8_t seed = utility::randomInt<24>();
  const Face top = static_cast<Face>(seed % 6);

  const auto get_next_valid_front_face = [&](const uint8_t& start) {
    for (uint8_t i = start; i < 6; ++i) {
      const Face face = static_cast<Face>(i);
      if (CubeOrientation{top, face}.isValid()) return face;
    }
    // this should never happen
    throw std::logic_error("No possible front faces left!");
  };

  Face front = get_next_valid_front_face(0);
  for (uint8_t i = 0; i < seed / 6; ++i)  // seed / 6 is in [0, 4)
    front = get_next_valid_front_face(static_cast<uint8_t>(front) + 1);

  return CubeOrientation{top, front};
}

CubeOrientation CubeOrientation::inv() const {
  using enum Face;
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
  for (size_t i = 0; i < static_cast<uint8_t>(cubeRotation.rotation_amount);
       i++) {
    switch (cubeRotation.rotation_axis) {
      case RotationAxis::X: {
        Face new_top = product.front;
        Face new_front = product.getBottomFace();
        product.top = new_top;
        product.front = new_front;
        break;
      }
      case RotationAxis::Y:
        product.front = product.getRightFace();
        break;
      case RotationAxis::Z:
        product.top = product.getLeftFace();
        break;
    }
  }
  return product;
}

void CubeOrientation::operator*=(const CubeRotation& cubeRotation) {
  (*this) = (*this) * cubeRotation;
}

std::pair<std::optional<CubeRotation>, std::optional<CubeRotation>>
CubeOrientation::solve() const {
  if (top == Face::U) {
    // only 1 CubeRotation required about Y axis
    switch (front) {
      case Face::F:
        return {{}, {}};  // already solved
      case Face::R:
        return {CubeRotation::parse("Y'").second, {}};
      case Face::B:
        return {CubeRotation::parse("Y2").second, {}};
      case Face::L:
        return {CubeRotation::parse("Y").second, {}};
      default:
        throw std::logic_error("Invalid CubeOrientation state!");
    }
  }
  if (front == Face::F) {
    // only 1 CubeRotation required about Z axis
    switch (top) {
        // case Face::U is not possible since it would be detected in the
        // previous conditional
      case Face::R:
        return {CubeRotation::parse("Z").second, {}};
      case Face::D:
        return {CubeRotation::parse("Z2").second, {}};
      case Face::L:
        return {CubeRotation::parse("Z'").second, {}};
      default:
        throw std::logic_error("Invalid CubeOrientation state!");
    }
  }
  // check if this can be solved with 1 rotation about X axis
  if (top == Face::B && front == Face::U)
    return {CubeRotation::parse("X").second, {}};
  if (top == Face::D && front == Face::B)
    return {CubeRotation::parse("X2").second, {}};
  if (top == Face::F && front == Face::D)
    return {CubeRotation::parse("X'").second, {}};
  // 2 CubeRotations required
  CubeRotation first;
  switch (top) {
      // case Face::U is not possible since it would be detected in the first
      // conditional
    case Face::F:
      first = CubeRotation::parse("X'").second;
      break;
    case Face::R:
      first = CubeRotation::parse("Z").second;
      break;
    case Face::B:
      first = CubeRotation::parse("X").second;
      break;
    case Face::L:
      first = CubeRotation::parse("Z'").second;
      break;
    case Face::D:
      first = CubeRotation::parse("X2").second;  // could also use Z2
      break;
    default:
      throw std::logic_error("Unknown enum value!");
  }
  const auto [second, third] = ((*this) * first).solve();
  assert(!third);
  return {first, second};
}

Face CubeOrientation::apply(const Face& face) const {
  switch (face) {
    case Face::U:
      return top;
    case Face::F:
      return front;
    case Face::R:
      return getRightFace();
    case Face::B:
      return getBackFace();
    case Face::L:
      return getLeftFace();
    case Face::D:
      return getBottomFace();
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

std::pair<Slice, bool> CubeOrientation::apply(const Slice& slice) const {
  return fromRotationFace(apply(getRotationFace(slice)));
}

Turn CubeOrientation::apply(const Turn& turn) const {
  return Turn{apply(turn.face), turn.rotation_amount};
}

SliceTurn CubeOrientation::apply(const SliceTurn& slice_turn) const {
  const auto [new_slice, reversed] = apply(slice_turn.slice);
  return SliceTurn{new_slice, reversed ? -slice_turn.rotation_amount
                                       : slice_turn.rotation_amount};
}

WideTurn CubeOrientation::apply(const WideTurn& wide_turn) const {
  return WideTurn{apply(wide_turn.face), wide_turn.rotation_amount};
}

Move CubeOrientation::apply(const Move& move) const {
  if (move.isTurn())
    return Move{apply(move.getTurn())};
  else if (move.isSliceTurn())
    return Move{apply(move.getSliceTurn())};
  else if (move.isWideTurn())
    return Move{apply(move.getWideTurn())};
  else {
    assert(move.isCubeRotation());
    return Move{apply(move.getCubeRotation())};
  }
}

CubeRotation CubeOrientation::apply(const CubeRotation& cubeRotation) const {
  // TODO: what is this lol
  return CubeRotation();
}
