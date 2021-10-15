#include "Turn.h"

std::string Turn::toStr() const {
    // This syntax tells C++ to use the functions in the global namespace instead of recursion
    // Not sure why this is needed since the functions have different signatures
    return ::toStr(face) + ::toStr(rotationAmount);
}
