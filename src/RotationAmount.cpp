#include "RotationAmount.h"

std::string toStr(const RotationAmount &rotationAmount) {
    switch (rotationAmount) {
        case NONE:
        case CLOCKWISE:
            return "";
        case HALF_TURN:
            return "2";
        case COUNTERCLOCKWISE:
            return "'";
    }
}

RotationAmount operator+(const RotationAmount &first, const RotationAmount &second) {
    uint8_t clockwiseTurns = (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 4;
    return static_cast<RotationAmount>(clockwiseTurns);
}

std::pair<int, RotationAmount> parseRotationAmount(const std::string &str) {
    if (str.empty()) return {0, CLOCKWISE};
    std::string first_char = str.substr(0, 1);
    if (first_char == "2") return {1, HALF_TURN};
    else if (first_char == "'") return {1, COUNTERCLOCKWISE};
    else return {0, CLOCKWISE};
}
