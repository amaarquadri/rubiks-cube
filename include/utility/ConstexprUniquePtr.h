#pragma once

#include <utility>

/**
 * TODO: deprecate once std::unique_ptr is constexpr enabled
 */
template <class T>
class ConstexprUniquePtr {
 private:
  T* ptr;

 public:
  constexpr ConstexprUniquePtr() : ptr(nullptr) {}

  explicit constexpr ConstexprUniquePtr(T* const ptr) : ptr(ptr) {}

  constexpr ConstexprUniquePtr(const ConstexprUniquePtr& other) = delete;

  constexpr ConstexprUniquePtr& operator=(const ConstexprUniquePtr& other) =
      delete;

  constexpr ConstexprUniquePtr(ConstexprUniquePtr&& other) noexcept {
    ptr = std::exchange(other.ptr, nullptr);
  }

  constexpr ConstexprUniquePtr& operator=(ConstexprUniquePtr&& other) noexcept {
    delete ptr;
    ptr = std::exchange(other.ptr, nullptr);
  }

  constexpr T* get() const { return ptr; }

  constexpr T* operator->() const { return ptr; }

  constexpr T& operator*() const { return *ptr; }

  constexpr ~ConstexprUniquePtr() { delete ptr; }
};
