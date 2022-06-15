#include "SolveAttemptParsingUtils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace utility {
std::pair<std::string, std::string> loadScrambleSolve(
    const std::string& file_name) {
  std::ifstream file(file_name);
  std::stringstream buffer;
  buffer << file.rdbuf();
  const std::string contents = buffer.str();

  size_t scramble_length = 0;
  while (scramble_length < contents.size() && contents[scramble_length] != '\n')
    scramble_length++;

  if (scramble_length == contents.size())
    throw std::invalid_argument(
        "No new line characters! Cannot distinguish scramble from solve!");

  return {contents.substr(0, scramble_length),
          contents.substr(scramble_length + 1,
                          contents.size() - scramble_length - 1)};
}
}  // namespace utility