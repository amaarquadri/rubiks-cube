#pragma once

#include "MathUtils.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <tuple>

namespace utility {
template <typename DataStore, typename T, size_t capacity>
class ForwardStaticVector {
 public:
  friend DataStore;
  using size_type = get_size_type_t<capacity>;
  using iterator = typename std::array<T, capacity>::iterator;
  using const_iterator = typename std::array<T, capacity>::const_iterator;
  using reverse_iterator = typename std::array<T, capacity>::reverse_iterator;
  using const_reverse_iterator =
      typename std::array<T, capacity>::const_reverse_iterator;

 private:
  explicit constexpr ForwardStaticVector(DataStore& data) : data(data) {}

  DataStore& data;

 public:
  constexpr size_type getSize() const { return data.forward_size; }

  [[nodiscard]] constexpr bool isEmpty() const {
    return data.forward_size == 0;
  }

  [[nodiscard]] constexpr bool isFull() const { return data.isFull(); }

  constexpr const T& operator[](const size_type& idx) const {
    assert(idx < data.forward_size);
    return data.values[idx];
  }

  constexpr T& operator[](const size_type& idx) {
    assert(idx < data.forward_size);
    return data.values[idx];
  }

  constexpr const_iterator cbegin() const { return data.values.cbegin(); }

  constexpr const_iterator begin() const { return data.values.cbegin(); }

  constexpr iterator begin() { return data.values.begin(); }

  constexpr const_iterator cend() const {
    return data.values.cbegin() + data.forward_size;
  }

  constexpr const_iterator end() const {
    return data.values.cbegin() + data.forward_size;
  }

  constexpr iterator end() { return data.values.begin() + data.forward_size; }

  constexpr const_reverse_iterator crbegin() const {
    return data.values.crbegin() + (capacity - data.forward_size);
  }

  constexpr const_reverse_iterator rbegin() const {
    return data.values.crbegin() + (capacity - data.forward_size);
  }

  constexpr reverse_iterator rbegin() {
    return data.values.rbegin() + (capacity - data.forward_size);
  }

  constexpr const_reverse_iterator crend() const { return data.values.crend(); }

  constexpr const_reverse_iterator rend() const { return data.values.crend(); }

  constexpr reverse_iterator rend() { return data.values.rend(); }

  constexpr const T& front() const {
    assert(!isEmpty());
    return data.values.front();
  }

  constexpr T& front() {
    assert(!isEmpty());
    return data.values.front();
  }

  constexpr const T& back() const {
    assert(!isEmpty());
    return data.values[data.forward_size - 1];
  }

  constexpr T& back() {
    assert(!isEmpty());
    return data.values[data.forward_size - 1];
  }

  constexpr void push_back(const T& t) {
    assert(!isFull());
    data.values[data.forward_size] = t;
    ++data.forward_size;
  }

  constexpr void pop_back() {
    assert(!isEmpty());
    --data.forward_size;
  }

  constexpr void clear() { data.forward_size = 0; }
};

template <typename DataStore, typename T, size_t capacity>
class BackwardStaticVector {
 public:
  friend DataStore;
  using size_type = get_size_type_t<capacity>;
  using iterator = typename std::array<T, capacity>::reverse_iterator;
  using const_iterator =
      typename std::array<T, capacity>::const_reverse_iterator;
  using reverse_iterator = typename std::array<T, capacity>::iterator;
  using const_reverse_iterator =
      typename std::array<T, capacity>::const_iterator;

 private:
  explicit constexpr BackwardStaticVector(DataStore& data) : data(data) {}

  DataStore& data;

 public:
  constexpr size_type getSize() const { return data.backward_size; }

  [[nodiscard]] constexpr bool isEmpty() const {
    return data.backward_size == 0;
  }

  [[nodiscard]] constexpr bool isFull() const { return data.isFull(); }

  constexpr const T& operator[](const size_type& idx) const {
    assert(idx < data.backward_size);
    return data.values[capacity - idx - 1];
  }

  constexpr T& operator[](const size_type& idx) {
    assert(idx < data.backward_size);
    return data.values[capacity - idx - 1];
  }

  constexpr const_iterator cbegin() const { return data.values.crbegin(); }

  constexpr const_iterator begin() const { return data.values.crbegin(); }

  constexpr iterator begin() { return data.values.rbegin(); }

  constexpr const_iterator cend() const {
    return data.values.crbegin() + data.backward_size;
  }

  constexpr const_iterator end() const {
    return data.values.crbegin() + data.backward_size;
  }

  constexpr iterator end() { return data.values.rbegin() + data.backward_size; }

  constexpr const_reverse_iterator crbegin() const {
    return data.values.cbegin() + (capacity - data.backward_size);
  }

  constexpr const_reverse_iterator rbegin() const {
    return data.values.cbegin() + (capacity - data.backward_size);
  }

  constexpr reverse_iterator rbegin() {
    return data.values.begin() + (capacity - data.backward_size);
  }

  constexpr const_reverse_iterator crend() const { return data.values.cend(); }

  constexpr const_reverse_iterator rend() const { return data.values.cend(); }

  constexpr reverse_iterator rend() { return data.values.end(); }

  constexpr const T& front() const {
    assert(!isEmpty());
    return data.values.back();
  }

  constexpr T& front() {
    assert(!isEmpty());
    return data.values.back();
  }

  constexpr const T& back() const {
    assert(!isEmpty());
    return data.values[capacity - data.backward_size];
  }

  constexpr T& back() {
    assert(!isEmpty());
    return data.values[capacity - data.backward_size];
  }

  constexpr void push_back(const T& t) {
    assert(!isFull());
    data.values[capacity - data.backward_size - 1] = t;
    ++data.backward_size;
  }

  constexpr void pop_back() {
    assert(!isEmpty());
    --data.backward_size;
  }

  constexpr void clear() { data.backward_size = 0; }
};

template <typename T, size_t capacity>
class BidirectionalStaticVector {
 public:
  using size_type = get_size_type_t<capacity>;
  using DataStore = BidirectionalStaticVector<T, capacity>;
  using ForwardVector = ForwardStaticVector<DataStore, T, capacity>;
  using BackwardVector = BackwardStaticVector<DataStore, T, capacity>;
  friend ForwardVector;
  friend BackwardVector;

 private:
  BidirectionalStaticVector() = default;

  std::array<T, capacity> values{};
  size_type forward_size{0};
  size_type backward_size{0};

  [[nodiscard]] constexpr bool isFull() const {
    return forward_size + backward_size == capacity;
  }

 public:
  static constexpr std::tuple<DataStore, ForwardVector, BackwardVector> make() {
    DataStore data{};
    ForwardVector forward{data};
    BackwardVector backward{data};
    return {data, forward, backward};
  }
};
}  // namespace utility
