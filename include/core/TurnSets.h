#pragma once

#include "Face.h"
#include "RotationAmount.h"
#include "Turn.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>

namespace detail {
template <uint8_t restricted_faces>
struct valid_restricted_faces {
  static constexpr bool value =
      restricted_faces <= 6 && restricted_faces % 2 == 0;
};

template <uint8_t restricted_faces>
static constexpr bool valid_restricted_faces_v =
    valid_restricted_faces<restricted_faces>::value;

template <uint8_t restricted_faces>
  requires valid_restricted_faces_v<restricted_faces>
static consteval std::array<Face, restricted_faces> getRestrictedFaces() {
  using enum Face;
  if constexpr (restricted_faces == 0)
    return {};
  else if constexpr (restricted_faces == 2)
    return {F, B};
  else if constexpr (restricted_faces == 4)
    return {F, B, R, L};
  else
    return {F, B, R, L, U, D};
}

template <uint8_t restricted_faces>
  requires valid_restricted_faces_v<restricted_faces>
static consteval std::array<Face, 6 - restricted_faces> getUnrestrictedFaces() {
  /** static **/ constexpr auto RestrictedFaces =
      getRestrictedFaces<restricted_faces>();
  using enum Face;
  std::array<Face, 6 - restricted_faces> unrestricted_faces;
  uint8_t i = 0;
  for (const Face& face : {F, R, U, B, L, D})
    if (std::none_of(RestrictedFaces.begin(), RestrictedFaces.end(),
                     [&](const Face& f) { return f == face; }))
      unrestricted_faces[i++] = face;
  assert(i == unrestricted_faces.size());
  return unrestricted_faces;
}

template <uint8_t restricted_faces>
  requires valid_restricted_faces_v<restricted_faces>
static consteval auto getPossibleTurns() {
  /** static **/ constexpr uint8_t PossibleTurnsCount =
      restricted_faces + 3 * (6 - restricted_faces);

  std::array<Turn, PossibleTurnsCount> possible_turns;
  uint8_t i = 0;
  for (const Face& unrestricted_face : getUnrestrictedFaces<restricted_faces>())
    for (const RotationAmount& rotation_amount :
         {RotationAmount::Clockwise, RotationAmount::HalfTurn,
          RotationAmount::Counterclockwise})
      possible_turns[i++] = Turn{unrestricted_face, rotation_amount};
  for (const Face& restricted_face : getRestrictedFaces<restricted_faces>())
    possible_turns[i++] = Turn{restricted_face, RotationAmount::HalfTurn};
  assert(i == PossibleTurnsCount);
  return possible_turns;
}
}  // namespace detail

static constexpr auto AllPossibleTurns = detail::getPossibleTurns<0>();

/**
 * Turns that maintain edge orientation.
 */
static constexpr auto EdgeOrientationPreservingTurns =
    detail::getPossibleTurns<2>();

/**
 * Turns that maintain domino reduction.
 */
static constexpr auto DominoReductionPreservingTurns =
    detail::getPossibleTurns<4>();

/**
 * Turns that maintain half turn reduction.
 */
static constexpr auto HalfTurnReductionPreservingTurns =
    detail::getPossibleTurns<6>();
