#pragma once

#include <cstddef>

/**
 * Note that this implementation is not thread safe!
 * TODO: deprecate once std::shared_ptr is constexpr enabled
 */
template <class T>
class ConstexprSharedPtr {
 private:
  T* ptr;
  size_t* ref_count;

 public:
  constexpr ConstexprSharedPtr() : ptr(nullptr), ref_count(new size_t(0)) {}

  explicit constexpr ConstexprSharedPtr(T* const ptr)
      : ptr(ptr), ref_count(new size_t(1)) {}

  constexpr ConstexprSharedPtr(const ConstexprSharedPtr& other) {
    ptr = other.ptr;
    ref_count = other.ref_count;
    if (ptr != nullptr) ++(*ref_count);
  }

  constexpr ConstexprSharedPtr& operator=(const ConstexprSharedPtr& other) {
    if (this == &other) return *this;
    cleanup();
    ptr = other.ptr;
    ref_count = other.ref_count;
    if (ptr != nullptr) ++(*ref_count);
    return *this;
  }

  constexpr ConstexprSharedPtr(ConstexprSharedPtr&& other) noexcept {
    ptr = other.ptr;
    ref_count = other.ref_count;
    other.ptr = nullptr;
    other.ref_count = nullptr;
  }

  constexpr ConstexprSharedPtr& operator=(ConstexprSharedPtr&& other) noexcept {
    cleanup();
    ptr = other.ptr;
    ref_count = other.ref_count;
    other.ptr = nullptr;
    other.ref_count = nullptr;
  }

  [[nodiscard]] constexpr size_t get_count() const { return *ref_count; }

  constexpr T* get() const { return ptr; }

  constexpr T* operator->() const { return ptr; }

  constexpr T& operator*() const { return *ptr; }

  constexpr ~ConstexprSharedPtr() { cleanup(); }

 private:
  constexpr void cleanup() {
    if (ref_count == nullptr) return;  // this is a move-from shared_ptr
    --(*ref_count);
    if (*ref_count == 0) {
      delete ptr;
      delete ref_count;
    }
  }
};
