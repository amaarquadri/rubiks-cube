#pragma once

namespace utility {
template <typename T>
T factorial(const T& n) {
  T result = static_cast<T>(1);
  // decrement instead of incrementing in the loop to avoid an infinite loop in
  // the case where n is the maximum value
  for (T val = n; val >= static_cast<T>(2); --val) result *= val;
  return result;
}
}  // namespace utility
