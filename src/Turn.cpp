#include "Turn.h"

std::string Turn::toStr() const {
    return toStr(face) + toStr(rotationAmount);
}
