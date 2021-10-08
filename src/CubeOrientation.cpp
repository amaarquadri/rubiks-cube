#include "CubeOrientation.h"

bool CubeOrientation::operator==(const CubeOrientation &other) const {
    return top == other.top && front == other.front;
}

Colour CubeOrientation::getRightFaceColour() const {
    // TODO
    return RED;
}

Colour CubeOrientation::getBackFaceColour() const {
    return getOpposite(front);
}

Colour CubeOrientation::getLeftFaceColour() const {
    // TODO
    return RED;
}

Colour CubeOrientation::getBottomFaceColour() const {
    return getOpposite(top);
}
