#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>

namespace utility {

/**
 * A heap allocated array with a compile-time-known size.
 * It's like a heap allocated version of std::array.
 * Typically this is useful for std::arrays that are too large to fit on the
 * stack. It has the benefit over std::vector since it doesn't need to store a
 * size or have extra logic for an exponential dynamic size growth.
 * TODO: is there a std implementation of this?
 */
template <typename T, size_t n>
class heap_array {
 public:
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
  T* const ptr;

 public:
  constexpr heap_array() : ptr(new T[n]) {}

  constexpr ~heap_array() { delete[] ptr; }

  [[nodiscard]] constexpr size_type size() const { return n; }

  [[nodiscard]] constexpr size_type max_size() const { return n; }

  [[nodiscard]] constexpr bool empty() const { return size() == 0; }

  constexpr pointer data() { return ptr; }

  constexpr const_pointer data() const { return ptr; }

  constexpr reference operator[](const size_type& idx) {
    assert(idx < size());
    return *(ptr + idx);
  }

  constexpr const_reference operator[](const size_type& idx) const {
    assert(idx < size());
    return *(ptr + idx);
  }

  constexpr reference at(const size_type& idx) {
    if (idx >= size()) throw std::logic_error("Out of bounds!");
    return *(ptr + idx);
  }

  constexpr const_reference at(const size_type& idx) const {
    if (idx >= size()) throw std::logic_error("Out of bounds!");
    return *(ptr + idx);
  }

  constexpr reference front() {
    assert(!empty());
    return (*this)[0];
  }

  constexpr const_reference front() const {
    assert(!empty());
    return (*this)[0];
  }

  constexpr reference back() {
    assert(!empty());
    return (*this)[n - 1];
  }

  constexpr const_reference back() const {
    assert(!empty());
    return (*this)[n - 1];
  }

  constexpr iterator begin() { return iterator{ptr}; }

  constexpr const_iterator begin() const { return const_iterator{ptr}; }

  constexpr const_iterator cbegin() const { return const_iterator{ptr}; }

  constexpr iterator end() { return iterator{ptr + n}; }

  constexpr const_iterator end() const { return const_iterator{ptr + n}; }

  constexpr const_iterator cend() const { return const_iterator{ptr + n}; }

  constexpr reverse_iterator rbegin() { return end(); }

  constexpr const_reverse_iterator rbegin() const { return cend(); }

  constexpr const_reverse_iterator crbegin() const { return cend(); }

  constexpr reverse_iterator rend() { return begin(); }

  constexpr const_reverse_iterator rend() const { return cbegin(); }

  constexpr const_reverse_iterator crend() const { return cbegin(); }

  constexpr void fill(const T& value) { std::fill_n(begin(), size(), value); }
};

}  // namespace utility
