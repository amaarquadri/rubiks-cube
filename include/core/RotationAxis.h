#pragma once

#include <cstdint>
#include <string>

enum class RotationAxis : uint8_t { X, Y, Z };

std::string toStr(const RotationAxis& rotationAxis);

/**
 * @brief Returns the number of characters consumed and the parsed RotationAxis.
 * If it was not possible to parse a RotationAxis, then the number of characters
 * consumed will be zero.
 */
std::pair<size_t, RotationAxis> parseRotationAxis(const std::string& str);
