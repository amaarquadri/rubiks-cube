#pragma once

#include <string>
#include <utility>

namespace utility {
/**
 * @brief Reads from the given file and parses the contents into a scramble
 * (which must be on the first line) and the solve (which must be on subsequent
 * lines).
 */
std::pair<std::string, std::string> loadScrambleSolve(
    const std::string& file_name);
}  // namespace utility
