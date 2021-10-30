#include "CubeOrientation.h"

bool CubeOrientation::operator==(const CubeOrientation &other) const {
    return top == other.top && front == other.front;
}

Colour CubeOrientation::getRightFaceColour() const {
    return getRight(top, front);
}

Colour CubeOrientation::getBackFaceColour() const {
    return getOpposite(front);
}

Colour CubeOrientation::getLeftFaceColour() const {
    return getLeft(top, front);
}

Colour CubeOrientation::getBottomFaceColour() const {
    return getOpposite(top);
}

CubeOrientation CubeOrientation::operator*(const CubeRotation &cubeRotation) {
    CubeOrientation product{top, front};
    for (int i = 0; i < static_cast<uint8_t>(cubeRotation.rotationAmount); i++) {
        switch (cubeRotation.rotationAxis) {
            case X: {
                Colour new_top = product.front;
                Colour new_front = product.getBottomFaceColour();
                product.top = new_top;
                product.front = new_front;
                break;
            }
            case Y:
                product.front = product.getRightFaceColour();
                break;
            case Z:
                product.top = product.getLeftFaceColour();
                break;
        }
    }
    return product;
}
