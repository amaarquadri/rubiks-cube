#include "RotationAxis.h"


std::string toStr(const RotationAxis &rotationAxis) {
    switch (rotationAxis) {
        case X:
            return "X";
        case Y:
            return "Y";
        case Z:
            return "Z";
    }
}

std::pair<int, RotationAxis> parseRotationAxis(const std::string &str) {
    if (str.empty()) return {0, {}};
    char first_char = str[0];
    if (first_char == 'X' || first_char == 'x') return {1, X};
    else if (first_char == 'Y' || first_char == 'y') return {1, Y};
    else if (first_char == 'Z' || first_char == 'z') return {1, Z};
    else return {0, {}};
}
