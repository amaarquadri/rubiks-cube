#pragma once

#include "MathUtils.h"
#include "heap_array.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace utility {
template <typename T, size_t capacity, bool use_heap = false>
class StaticVector {
 public:
  using size_type = get_size_type_t<capacity>;
  using iterator = typename std::array<T, capacity>::iterator;
  using const_iterator = typename std::array<T, capacity>::const_iterator;
  using reverse_iterator = typename std::array<T, capacity>::reverse_iterator;
  using const_reverse_iterator =
      typename std::array<T, capacity>::const_reverse_iterator;

 private:
  pick_array_t<T, capacity, use_heap> values{};
  size_type size{0};

 public:
  constexpr size_type getSize() const { return size; }

  [[nodiscard]] constexpr bool isEmpty() const { return size == 0; }

  [[nodiscard]] constexpr bool isFull() const { return size == capacity; }

  constexpr const T& operator[](const size_type& idx) const {
    assert(idx < size);
    return values[idx];
  }

  constexpr T& operator[](const size_type& idx) {
    assert(idx < size);
    return values[idx];
  }

  constexpr const_iterator cbegin() const { return values.cbegin(); }

  constexpr const_iterator begin() const { return values.cbegin(); }

  constexpr iterator begin() { return values.begin(); }

  constexpr const_iterator cend() const { return values.cbegin() + size; }

  constexpr const_iterator end() const { return values.cbegin() + size; }

  constexpr iterator end() { return values.begin() + size; }

  constexpr const_reverse_iterator crbegin() const {
    return values.crbegin() + (capacity - size);
  }

  constexpr const_reverse_iterator rbegin() const {
    return values.crbegin() + (capacity - size);
  }

  constexpr reverse_iterator rbegin() {
    return values.rbegin() + (capacity - size);
  }

  constexpr const_reverse_iterator crend() const { return values.crend(); }

  constexpr const_reverse_iterator rend() const { return values.crend(); }

  constexpr reverse_iterator rend() { return values.rend(); }

  constexpr const T& front() const {
    assert(!isEmpty());
    return values.front();
  }

  constexpr T& front() {
    assert(!isEmpty());
    return values.front();
  }

  constexpr const T& back() const {
    assert(!isEmpty());
    return values[size - 1];
  }

  constexpr T& back() {
    assert(!isEmpty());
    return values[size - 1];
  }

  constexpr void push_back(const T& t) {
    assert(!isFull());
    values[size] = t;
    ++size;
  }

  constexpr void pop_back() {
    assert(!isEmpty());
    --size;
  }

  constexpr void clear() { size = 0; }
};
}  // namespace utility
