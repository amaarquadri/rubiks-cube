#pragma once

#include <cstddef>
#include <type_traits>

namespace utility {
static constexpr std::array<size_t, 8> PowersOf3{1,  3,   9,   27,
                                                 81, 243, 729, 2187};

template <typename T>
constexpr T factorial(const T& n) {
  T result = static_cast<T>(1);
  // decrement instead of incrementing in the loop to avoid an infinite loop in
  // the case where n is the maximum value
  for (T val = n; val >= static_cast<T>(2); --val) result *= val;
  return result;
}

template <typename T1, typename T2>
constexpr auto pow(const T1& base, const T2& exponent) {
  using R = std::common_type_t<T1, T2>;
  R result = static_cast<R>(1);
  for (size_t i = 0; i < exponent; ++i) result *= base;
  return result;
}

/**
 * Returns zero if n < k and returns one if n == k.
 */
template <typename T1, typename T2>
constexpr std::common_type_t<T1, T2> nChooseK(const T1& n, const T2& k) {
  using R = std::common_type_t<T1, T2>;
  if (n < k) return 0;
  R result = 1;
  for (R i = 0; i < k; ++i) {
    result *= (n - i);
    result /= (i + 1);
  }
  return result;
}

// Test cases
static_assert(factorial(5) == 120);
static_assert(pow(3, 7) == 2187);
static_assert(nChooseK(12, 4) == 495);
static_assert(nChooseK(8, 4) == 70);
}  // namespace utility
