#include "CubeOrientation.h"

bool CubeOrientation::operator==(const CubeOrientation &other) const {
    return top == other.top && front == other.front;
}

Face CubeOrientation::getRightFace() const {
    return getRight(top, front);
}

Face CubeOrientation::getBackFace() const {
    return getOpposite(front);
}

Face CubeOrientation::getLeftFace() const {
    return getLeft(top, front);
}

Face CubeOrientation::getBottomFace() const {
    return getOpposite(top);
}



CubeOrientation CubeOrientation::identity() {
    return {U, F};
}

CubeOrientation CubeOrientation::operator*(const CubeRotation &cubeRotation) const {
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

Face CubeOrientation::apply(const Face &face) const {
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
            return getRightFace();
    }
}

std::pair<Slice, bool> CubeOrientation::apply(const Slice &slice) const {
    return fromRotationFace(apply(asRotationFace(slice)));
}
