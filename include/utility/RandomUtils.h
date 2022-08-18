#pragma once

#include <chrono>
#include <cstddef>
#include <random>

namespace utility {
const unsigned seed =  // NOLINT(cert-err58-cpp)
    std::chrono::system_clock::now().time_since_epoch().count();
auto random_engine =  // NOLINT(cert-err58-cpp)
    std::default_random_engine(seed);

/**
 * @return A random integer in [0, n).
 */
template <size_t n>
size_t randomInt() {
  static std::uniform_int_distribution<size_t> distribution(0, n - 1);
  return distribution(random_engine);
}

bool randomBool() { return randomInt<1>(); }
}  // namespace utility
