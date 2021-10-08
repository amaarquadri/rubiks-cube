#include "Face.h"
#include "CornerLocation.h"
#include "Cube.h"

Face getOpposite(const Face &face) {
    switch (face) {
        case U:
            return D;
        case F:
            return B;
        case R:
            return L;
        case B:
            return F;
        case L:
            return R;
        case D:
            return U;
    }
}

Face getLeft(const Face &top, const Face &front) {
    for (CornerLocation corner : Cube::CORNER_LOCATION_ORDER) {
        if (top == corner.first && front == corner.second) return corner.third;
    }
}

Face getRight(const Face &top, const Face &front) {
    return getOpposite(getLeft(top, front));
}

std::string toStr(const Face &face) {
    switch (face) {
        case U:
            return "U";
        case F:
            return "F";
        case R:
            return "R";
        case B:
            return "B";
        case L:
            return "L";
        case D:
            return "D";
    }
}