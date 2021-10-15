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
