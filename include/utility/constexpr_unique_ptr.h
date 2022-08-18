#pragma once

#include <utility>

/**
 * TODO: deprecate once std::unique_ptr is constexpr enabled
 */
template <class T>
class constexpr_unique_ptr {
 private:
  T* ptr;

 public:
  constexpr constexpr_unique_ptr() : ptr(nullptr) {}

  explicit constexpr constexpr_unique_ptr(T* const ptr) : ptr(ptr) {}

  constexpr constexpr_unique_ptr(const constexpr_unique_ptr& other) = delete;

  constexpr constexpr_unique_ptr& operator=(const constexpr_unique_ptr& other) =
      delete;

  constexpr constexpr_unique_ptr(constexpr_unique_ptr&& other) noexcept {
    ptr = std::exchange(other.ptr, nullptr);
  }

  constexpr constexpr_unique_ptr& operator=(
      constexpr_unique_ptr&& other) noexcept {
    delete ptr;
    ptr = std::exchange(other.ptr, nullptr);
  }

  constexpr T* get() const { return ptr; }

  constexpr T* operator->() const { return ptr; }

  constexpr T& operator*() const { return ptr; }

  constexpr ~constexpr_unique_ptr() { delete ptr; }
};