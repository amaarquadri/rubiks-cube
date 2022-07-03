#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace utility {
struct Uint24Reference {
  uint8_t& high;
  uint8_t& middle;
  uint8_t& low;

  constexpr void operator=(const uint32_t& value) {
    assert(value < (1 << 24));
    high = static_cast<uint8_t>(value >> 16);
    middle = static_cast<uint8_t>(value >> 8);
    low = static_cast<uint8_t>(value);
  }

  constexpr operator uint32_t() const {
    return (static_cast<uint32_t>(high) << 16) +
           (static_cast<uint32_t>(middle) << 8) + static_cast<uint32_t>(low);
  }
};

class Uint24Iterator {
 private:
  uint8_t* data;

 public:
  using value_type = Uint24Reference;
  using difference_type = long long;

  constexpr Uint24Iterator(uint8_t* const data) : data(data) {}

  constexpr Uint24Iterator& operator++() {
    data += 3;
    return *this;
  }

  constexpr Uint24Iterator operator++(int) {
    const Uint24Iterator copy = *this;
    data += 3;
    return copy;
  }

  constexpr Uint24Iterator& operator--() {
    data -= 3;
    return *this;
  }

  constexpr Uint24Iterator operator--(int) {
    const Uint24Iterator copy = *this;
    data -= 3;
    return copy;
  }

  constexpr Uint24Iterator operator+(const size_t& step) {
    return Uint24Iterator{data + 3 * step};
  }

  constexpr Uint24Iterator operator-(const size_t& step) {
    return Uint24Iterator{data - 3 * step};
  }

  constexpr Uint24Iterator& operator+=(const size_t& step) {
    data += 3 * step;
    return *this;
  }

  constexpr Uint24Iterator& operator-=(const size_t& step) {
    data += 3 * step;
    return *this;
  }

  constexpr auto operator<=>(const Uint24Iterator&) const = default;

  constexpr difference_type operator-(const Uint24Iterator& other) const {
    const difference_type difference = data - other.data;
    assert(difference % 3 == 0);
    return difference / 3;
  }

  constexpr value_type operator*() {
    return value_type{*data, *(data + 1), *(data + 2)};
  }

  constexpr value_type operator[](const size_t& i) { return *((*this) + i); }
};

class Uint24ConstIterator {
 private:
  const uint8_t* data;

 public:
  using value_type = uint32_t;
  using difference_type = size_t;

  constexpr Uint24ConstIterator(const uint8_t* const data) : data(data) {}

  constexpr Uint24ConstIterator& operator++() {
    data += 3;
    return *this;
  }

  constexpr Uint24ConstIterator operator++(int) {
    const Uint24ConstIterator copy = *this;
    data += 3;
    return copy;
  }

  constexpr Uint24ConstIterator& operator--() {
    data -= 3;
    return *this;
  }

  constexpr Uint24ConstIterator operator--(int) {
    const Uint24ConstIterator copy = *this;
    data -= 3;
    return copy;
  }

  constexpr Uint24ConstIterator operator+(const size_t& step) {
    return Uint24ConstIterator{data + 3 * step};
  }

  constexpr Uint24ConstIterator operator-(const size_t& step) {
    return Uint24ConstIterator{data - 3 * step};
  }

  constexpr Uint24ConstIterator& operator+=(const size_t& step) {
    data += 3 * step;
    return *this;
  }

  constexpr Uint24ConstIterator& operator-=(const size_t& step) {
    data += 3 * step;
    return *this;
  }

  constexpr auto operator<=>(const Uint24ConstIterator&) const = default;

  constexpr difference_type operator-(const Uint24ConstIterator& other) const {
    const difference_type difference = data - other.data;
    assert(difference % 3 == 0);
    return difference / 3;
  }

  constexpr uint32_t operator*() const {
    return (static_cast<uint32_t>(*data) << 16) +
           (static_cast<uint32_t>(*(data + 1)) << 8) +
           static_cast<uint32_t>(*(data + 2));
  }

  constexpr value_type operator[](const size_t& i) { return *((*this) + i); }
};

template <size_t n>
class Uint24Array {
 private:
  std::array<uint8_t, 3 * n> data;

 public:
  using size_type = size_t;
  using reference = Uint24Reference;
  using const_reference = uint32_t;
  using iterator = Uint24Iterator;
  using const_iterator = Uint24ConstIterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr size_type size() const { return n; }

  constexpr size_type max_size() const { return n; }

  constexpr bool empty() const { return n == 0; }

  constexpr reference operator[](const size_type& i) {
    assert(i < n);
    return reference{data[3 * i], data[3 * i + 1], data[3 * i + 2]};
  };

  constexpr const_reference operator[](const size_type& i) const {
    return (static_cast<uint32_t>(data[3 * i]) << 16) +
           (static_cast<uint32_t>(data[3 * i + 1]) << 8) +
           static_cast<uint32_t>(data[3 * i + 2]);
  }

  constexpr reference at(const size_type& i) {
    if (i < n) throw std::invalid_argument("i is out of range!");
    return data[i];
  }

  constexpr const_reference at(const size_type& i) const {
    if (i < n) throw std::invalid_argument("i is out of range!");
    return data[i];
  }

  constexpr reference front() {
    assert(n >= 1);
    return reference{data[0], data[1], data[2]};
  }

  constexpr const_reference front() const {
    assert(n >= 1);
    return (static_cast<uint32_t>(data[0]) << 16) +
           (static_cast<uint32_t>(data[1]) << 8) +
           static_cast<uint32_t>(data[2]);
  }

  constexpr reference back() {
    assert(n >= 1);
    return reference{data[3 * n - 3], data[3 * n - 2], data[3 * n - 1]};
  }

  constexpr const_reference back() const {
    assert(n >= 1);
    return (static_cast<uint32_t>(data[3 * n - 3]) << 16) +
           (static_cast<uint32_t>(data[3 * n - 2]) << 8) +
           static_cast<uint32_t>(data[3 * n - 1]);
  }

  constexpr iterator begin() { return iterator{data.begin()}; }

  constexpr iterator end() { return iterator{data.end()}; }

  constexpr const_iterator begin() const {
    return const_iterator{data.cbegin()};
  }

  constexpr const_iterator end() const { return const_iterator{data.cend()}; }

  constexpr const_iterator cbegin() const {
    return const_iterator{data.cbegin()};
  }

  constexpr const_iterator cend() const { return const_iterator{data.cend()}; }

  constexpr reverse_iterator rbegin() { return reverse_iterator{data.end()}; }

  constexpr reverse_iterator rend() { return reverse_iterator{data.begin()}; }

  constexpr const_reverse_iterator rbegin() const {
    return const_reverse_iterator{data.cend()};
  }

  constexpr const_reverse_iterator rend() const {
    return const_reverse_iterator{data.cbegin()};
  }

  constexpr const_reverse_iterator crbegin() const {
    return const_reverse_iterator{data.cend()};
  }

  constexpr const_reverse_iterator crend() const {
    return const_reverse_iterator{data.cbegin()};
  }

  constexpr void fill(const uint32_t& value) {
    for (auto& element : *this) element = value;
  }
};
}  // namespace utility
