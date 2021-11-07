#pragma once

#include "CubeRotation.h"
#include "Face.h"
#include <cstdint>
#include <utility>

enum Slice : uint8_t {
    M, E, S
};

/**
 * @return The face that has the same rotation axis and direction as the given Slice
 */
Face getRotationFace(const Slice &slice);

/**
 * @brief Computes the RotationAxis that results from applying this Slice with a RotationAmount of CLOCKWISE
 * If the bool is true, then the RotationAxis is in the opposite direction
 */
std::pair<RotationAxis, bool> getRotationAxis(const Slice &slice);

/**
 * @breif Computes the slice that has the same rotation direction as the given Face
 * If the bool is true, then the slice is the opposite direction
 */
std::pair<Slice, bool> fromRotationFace(const Face &face);

std::string toStr(const Slice &face);

// returns the number of characters consumed and the parsed Slice
// if it was not possible to parse a Slice, then the number of characters consumed will be zero
std::pair<int, Slice> parseSlice(const std::string &str);
