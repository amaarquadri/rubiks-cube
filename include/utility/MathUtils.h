#pragma once

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace utility {
template <typename T>
constexpr T factorial(const T& n) {
  T result = 1;
  // decrement instead of incrementing in the loop to avoid an infinite loop in
  // the case where n is the maximum value
  for (T val = n; val >= 2; --val) result *= val;
  return result;
}

template <typename T1, typename T2>
constexpr auto pow(const T1& base, const T2& exponent) {
  using R = std::common_type_t<T1, T2>;
  R result = 1;
  for (size_t i = 0; i < exponent; ++i) result *= base;
  return result;
}

// These are used a bunch, so we'll just precompute them
static constexpr std::array<size_t, 8> PowersOf3{
    pow(3, 0), pow(3, 1), pow(3, 2), pow(3, 3),
    pow(3, 4), pow(3, 5), pow(3, 6), pow(3, 7)};

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

/**
 * Computes the number of required bits to store the specified number of
 * distinct values in an unsigned integer.
 */
template <std::unsigned_integral T>
constexpr uint8_t requiredBits(T value) {
  --value;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  if constexpr (8 * sizeof(T) > 32) value |= value >> 32;
  return std::popcount(value);
}
static_assert(requiredBits(32ull) == 5);
static_assert(requiredBits(45ull) == 6);

template <uint8_t bits>
struct get_smallest_unsigned_int {
  static_assert(bits <= 64);

  using type = decltype([]() {
    if constexpr (bits <= 8)
      return uint8_t{};
    else if constexpr (bits <= 16)
      return uint16_t{};
    else if constexpr (bits <= 32)
      return uint32_t{};
    else
      return uint64_t{};
  }());
};

template <uint8_t bits>
using get_smallest_unsigned_int_t =
    typename get_smallest_unsigned_int<bits>::type;

template <size_t capacity>
struct get_size_type {
  using type = decltype([]() {
    if constexpr (capacity < (1 << 8))
      return uint8_t{};
    else if constexpr (capacity < (1 << 16))
      return uint16_t{};
    else if constexpr (capacity < (1ull << 32))
      return uint32_t{};
    else
      return uint64_t{};
  }());
};

template <size_t capacity>
using get_size_type_t = typename get_size_type<capacity>::type;

// Test cases
static_assert(factorial(5) == 120);
static_assert(pow(3, 7) == 2187);
static_assert(nChooseK(12, 4) == 495);
static_assert(nChooseK(8, 4) == 70);
}  // namespace utility
