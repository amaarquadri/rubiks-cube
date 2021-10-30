#include "RotationAxis.h"


std::string toStr(const Face &face) {
    switch (face) {
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
    std::string first_char = str.substr(0, 1);
    if (first_char == "X") return {1, X};
    else if (first_char == "Y") return {1, Y};
    else if (first_char == "Z") return {1, Z};
    else return {0, {}};
}
