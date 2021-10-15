#include "CubeRotation.h"

// TODO: verify

Face CubeRotation::apply(const Face &face) const {
    switch (face) {
        case U:
            return newTop;
        case F:
            return newFront;
        case R:
            getRight(newTop, newFront);
        case B:
            return getOpposite(newFront);
        case L:
            return getLeft(newTop, newFront);
        case D:
            return getOpposite(newTop);
    }
}

CubeRotation CubeRotation::inv() const {
    return {apply(U), apply(F)};
}

CubeRotation CubeRotation::identity() {
    return CubeRotation{U, F};
}

CubeRotation CubeRotation::operator*(const CubeRotation &other) const {
    CubeRotation inverse = inv();
    return {inverse.apply(other.newTop), inverse.apply(other.newFront)};
}

CubeOrientation CubeRotation::operator*(const CubeOrientation &cubeOrientation) const {
    // TODO
    return cubeOrientation;
}

Turn CubeRotation::operator*(const Turn &turn) const {
    // TODO
    return turn;
}

std::string CubeRotation::toStr() const {
    // TODO
    return "";
}
