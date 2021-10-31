#pragma once

#include "Face.h"
#include "Slice.h"
#include "CubeRotation.h"
#include "Turn.h"
#include <utility>

struct CubeOrientation {
    Face top; // the face that is now on top
    Face front; // the face that is now in the front

    bool operator==(const CubeOrientation &other) const;

    [[nodiscard]] Face getRightFace() const;

    [[nodiscard]] Face getBackFace() const;

    [[nodiscard]] Face getLeftFace() const;

    [[nodiscard]] Face getBottomFace() const;

    static CubeOrientation identity();

    /**
     * @brief Applies the given CubeRotation to this CubeOrientation and returns the resulting CubeOrientation
     */
    [[nodiscard]] CubeOrientation operator*(const CubeRotation &cubeRotation) const;

    void operator*=(const CubeRotation &cubeRotation);

    /**
     * @brief Converts the given Face in this CubeOrientation to the equivalent Face in the identity CubeOrientation
     */
    [[nodiscard]] Face apply(const Face &face) const;

    /**
     * @brief Converts the given Slice in this CubeOrientation to the equivalent Slice in the identity CubeOrientation
     * If the bool is true, then the Slice is reversed
     */
    [[nodiscard]] std::pair<Slice, bool> apply(const Slice &slice) const;

    /**
     * @brief Converts the given Turn in this CubeOrientation to the equivalent Turn in the identity CubeOrientation
     */
    [[nodiscard]] Turn apply(const Turn &turn) const;
};
