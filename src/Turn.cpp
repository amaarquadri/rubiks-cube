#include "Turn.h"

Turn Turn::inv() const {
    return is_slice_turn ? Turn{slice, ::inv(rotationAmount)} : Turn{face, ::inv(rotationAmount)};
}

std::string Turn::toStr() const {
    // This syntax tells C++ to use the functions in the global namespace instead of recursion
    // Not sure why this is needed since the functions have different signatures
    return (is_slice_turn ? ::toStr(slice) : ::toStr(face)) + ::toStr(rotationAmount);
}

std::pair<int, Turn> Turn::parse(const std::string &str) {
    auto [consumed_for_face, face] = parseFace(str);
    if (consumed_for_face == 0) return {0, {}}; // not possible to parse
    std::string remaining = str.substr(consumed_for_face, str.size() - consumed_for_face);
    auto [consumed_for_rotation_amount, rotation_amount] = parseRotationAmount(remaining);
    return {consumed_for_face + consumed_for_rotation_amount, {face, rotation_amount}};
}
