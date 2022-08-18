#pragma once

#include "ConstexprSharedPtr.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>

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
class HeapArray {
 public:
  using underlying_array = std::array<T, n>;
  using pointer = typename underlying_array::pointer;
  using const_pointer = typename underlying_array::const_pointer;
  using reference = typename underlying_array::reference;
  using const_reference = typename underlying_array::const_reference;
  using size_type = typename underlying_array::size_type;
  using iterator = typename underlying_array::iterator;
  using const_iterator = typename underlying_array::const_iterator;
  using reverse_iterator = typename underlying_array::reverse_iterator;
  using const_reverse_iterator =
      typename underlying_array::const_reverse_iterator;

 private:
  const ConstexprSharedPtr<underlying_array> ptr{new underlying_array()};

 public:
  constexpr HeapArray() = default;

  constexpr HeapArray(const HeapArray<T, n>& other) : ptr(other.ptr) {}

  constexpr HeapArray<T, n>& operator=(const HeapArray<T, n>& other) {
    ptr = other.ptr;
  };

  constexpr HeapArray(HeapArray<T, n>&& other) noexcept
      : ptr(std::move(other.ptr)) {}

  constexpr HeapArray<T, n>& operator=(HeapArray<T, n>&& other) noexcept {
    ptr = std::move(other.ptr);
  }

  [[nodiscard]] constexpr size_type size() const { return ptr->size(); }

  [[nodiscard]] constexpr size_type max_size() const { return ptr->max_size(); }

  [[nodiscard]] constexpr bool empty() const { return ptr->empty(); }

  constexpr pointer data() { return ptr->data(); }

  constexpr const_pointer data() const { return ptr->data(); }

  constexpr reference operator[](const size_type& idx) {
    assert(idx < size());
    return (*ptr)[idx];
  }

  constexpr const_reference operator[](const size_type& idx) const {
    assert(idx < size());
    return (*ptr)[idx];
  }

  constexpr reference at(const size_type& idx) {
    if (idx >= size()) throw std::logic_error("Out of bounds!");
    return (*ptr)[idx];
  }

  constexpr const_reference at(const size_type& idx) const {
    if (idx >= size()) throw std::logic_error("Out of bounds!");
    return (*ptr)[idx];
  }

  constexpr reference front() {
    assert(!ptr->empty());
    return ptr->front();
  }

  constexpr const_reference front() const {
    assert(!ptr->empty());
    return ptr->front();
  }

  constexpr reference back() {
    assert(!ptr->empty());
    return ptr->back();
  }

  constexpr const_reference back() const {
    assert(!ptr->empty());
    return ptr->back();
  }

  constexpr iterator begin() { return ptr->begin(); }

  constexpr const_iterator begin() const { return ptr->begin(); }

  constexpr const_iterator cbegin() const { return ptr->cbegin(); }

  constexpr iterator end() { return ptr->end(); }

  constexpr const_iterator end() const { return ptr->end(); }

  constexpr const_iterator cend() const { return ptr->cend(); }

  constexpr reverse_iterator rbegin() { return ptr->rbegin(); }

  constexpr const_reverse_iterator rbegin() const { return ptr->rbegin(); }

  constexpr const_reverse_iterator crbegin() const { return ptr->crbegin(); }

  constexpr reverse_iterator rend() { return ptr->rend(); }

  constexpr const_reverse_iterator rend() const { return ptr->rend(); }

  constexpr const_reverse_iterator crend() const { return ptr->crend(); }

  constexpr void fill(const T& value) { ptr->fill(value); }
};

template <typename T, size_t n, bool use_heap = false>
struct pick_array {
  using type = std::conditional_t<use_heap, HeapArray<T, n>, std::array<T, n>>;
};

template <typename T, size_t n, bool use_heap = false>
using pick_array_t = typename pick_array<T, n, use_heap>::type;

}  // namespace utility
