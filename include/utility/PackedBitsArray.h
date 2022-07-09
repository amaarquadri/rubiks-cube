#pragma once

#include "MathUtils.h"
#include "Utils.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <variant>

namespace utility {
template <uint8_t bits>
struct PackedBitsReference {
  static_assert(bits > 0 && bits <= 64);

 private:
  static constexpr bool is_byte_aligned = bits % 8 == 0;
  static constexpr uint8_t full_bytes = bits / 8;
  static constexpr uint8_t extra_bits = bits % 8;

 public:
  using value_type = get_smallest_unsigned_int_t<bits>;
  using offset_type =
      std::conditional_t<is_byte_aligned, std::monostate, uint8_t>;

 private:
  uint8_t* const data;
  const offset_type offset{};

 public:
  PackedBitsReference(uint8_t* const data) : data(data) {}

  PackedBitsReference(uint8_t* const data, const offset_type& offset)
      : data(data), offset(offset) {}

 private:
  static constexpr uint8_t trailingOnes(const uint8_t& count) {
    assert(count <= 8);
    return utility::pow(2, count) - 1;
  }
  static_assert(trailingOnes(0) == 0 && trailingOnes(8) == 255);

  static constexpr uint8_t leadingOnes(const uint8_t& count) {
    assert(count <= 8);
    // need to explicitly cast since ~ performs type promotion
    return static_cast<uint8_t>(~trailingOnes(8 - count));
  }
  static_assert(leadingOnes(0) == 0 && leadingOnes(8) == 255);

  template <uint8_t offset_>
  [[gnu::always_inline]] constexpr void write_data(const value_type& value) {
    static_assert(offset_ < 8);
    if constexpr (bits < (8 - offset_)) {
      (*data) &= leadingOnes(offset_) + trailingOnes(8 - offset_ - bits);
      (*data) |= value << (8 - offset_ - bits);
      return;
    }
    // write the entire (8 - offset_) bits
    (*data) &= leadingOnes(offset_);
    (*data) |= static_cast<uint8_t>(value >> (bits - (8 - offset_)));
    if constexpr (bits == (8 - offset_)) return;

    constexprFor<1, full_bytes + 1>([&](auto i) {
      static constexpr uint8_t bits_seen = 8 * (i + 1) - offset_;
      if constexpr (bits < bits_seen) {
        // erase the high bits and write the new high bits
        (*(data + i)) &= trailingOnes(bits_seen - bits);
        (*(data + i)) |= static_cast<uint8_t>(value << (bits_seen - bits));
        return Break{};
      } else {
        // write the entire byte
        (*(data + i)) = static_cast<uint8_t>(value >> (bits - bits_seen));
        if constexpr (bits == bits_seen) return Break{};
      }
    });
  }

  template <uint8_t offset_>
  [[gnu::always_inline]] constexpr value_type read_data() const {
    static_assert(offset_ < 8);
    if constexpr (bits <= (8 - offset_))
      return ((*data) & trailingOnes(8 - offset_)) >> (8 - bits - offset_);

    value_type result =
        static_cast<value_type>((*data) & trailingOnes(8 - offset_))
        << (bits - (8 - offset_));
    constexprFor<1, full_bytes + 1>([&](auto i) {
      static constexpr uint8_t bits_seen = 8 * (i + 1) - offset_;
      if constexpr (bits <= bits_seen) {
        result |= static_cast<value_type>((*(data + i)) >> (bits_seen - bits));
        return Break{};
      } else
        result |= static_cast<value_type>(*(data + i)) << (bits - bits_seen);
    });
    return result;
  }

 public:
  constexpr void operator=(const value_type& value) {
    // special case for 64 since left shift will fail
    assert(bits == 64 || value < (1ull << bits));
    if constexpr (is_byte_aligned) {
      constexprFor<0, full_bytes>([&](auto i) {
        (*(data + i)) = static_cast<uint8_t>(value >> (bits - 8 * (i + 1)));
      });
    } else {
      assert(offset < 8);
      switch (offset) {
        case 0:
          write_data<0>(value);
          break;
        case 1:
          assert(extra_bits % 2 != 0);
          write_data<1>(value);
          break;
        case 2:
          assert(extra_bits % 4 != 0);
          write_data<2>(value);
          break;
        case 3:
          assert(extra_bits % 2 != 0);
          write_data<3>(value);
          break;
        case 4:
          assert(extra_bits != 0);
          write_data<4>(value);
          break;
        case 5:
          assert(extra_bits % 2 != 0);
          write_data<5>(value);
          break;
        case 6:
          assert(extra_bits % 4 != 0);
          write_data<6>(value);
          break;
        case 7:
          assert(extra_bits % 2 != 0);
          write_data<7>(value);
          break;
        default:
          throw std::logic_error("Invalid offset!");
      }
    }
  }

  constexpr operator value_type() const {
    if constexpr (is_byte_aligned) {
      value_type result = 0;
      constexprFor<0, full_bytes>([&](auto i) {
        result |= static_cast<value_type>(*(data + i)) << (bits - 8 * (i + 1));
      });
      return result;
    } else {
      assert(offset < 8);
      switch (offset) {
        case 0:
          return read_data<0>();
        case 1:
          assert(extra_bits % 2 != 0);
          return read_data<1>();
        case 2:
          assert(extra_bits % 4 != 0);
          return read_data<2>();
        case 3:
          assert(extra_bits % 2 != 0);
          return read_data<3>();
        case 4:
          assert(extra_bits != 0);
          return read_data<4>();
        case 5:
          assert(extra_bits % 2 != 0);
          return read_data<5>();
        case 6:
          assert(extra_bits % 4 != 0);
          return read_data<6>();
        case 7:
          assert(extra_bits % 2 != 0);
          return read_data<7>();
        default:
          throw std::logic_error("Invalid offset!");
      }
    }
  }
};

template <uint8_t bits, bool is_const_iterator = false>
class PackedBitsIteratorImpl {
  static_assert(bits > 0 && bits <= 64);

 private:
  static constexpr bool is_byte_aligned = bits % 8 == 0;
  static constexpr uint8_t full_bytes = bits / 8;
  static constexpr uint8_t extra_bits = bits % 8;

 public:
  using offset_type = typename PackedBitsReference<bits>::offset_type;
  using ptr_type =
      std::conditional<is_const_iterator, const uint8_t*, uint8_t*>;

 private:
  ptr_type data;
  offset_type offset{};  // must be in [0, 8)

 public:
  using value_type =
      std::conditional_t<is_const_iterator, get_smallest_unsigned_int_t<bits>,
                         PackedBitsReference<bits>>;
  using difference_type = long long;

  constexpr PackedBitsIteratorImpl(const ptr_type data) : data(data) {}

  constexpr PackedBitsIteratorImpl(const ptr_type data,
                                   const offset_type& offset)
      : data(data), offset(offset) {}

  constexpr PackedBitsIteratorImpl& operator++() {
    data += full_bytes;
    if constexpr (!is_byte_aligned) {
      offset += extra_bits;
      if (offset >= 8) {
        offset -= 8;
        ++data;
      }
    }
    return *this;
  }

  constexpr PackedBitsIteratorImpl operator++(int) {
    const PackedBitsIteratorImpl copy = *this;
    ++(*this);
    return copy;
  }

  constexpr PackedBitsIteratorImpl& operator--() {
    data -= full_bytes;
    if constexpr (!is_byte_aligned) {
      offset -= extra_bits;
      if (offset >= 8) {  // if offset underflow-ed
        offset += 8;      // re-overflow to within [0, 8)
        --data;
      }
    }
    return *this;
  }

  constexpr PackedBitsIteratorImpl operator--(int) {
    const PackedBitsIteratorImpl copy = *this;
    --(*this);
    return copy;
  }

  constexpr PackedBitsIteratorImpl operator+(const size_t& step) const {
    if constexpr (is_byte_aligned) {
      return PackedBitsIteratorImpl<bits>{data + step * full_bytes};
    } else {
      const size_t overflow_bits = step * extra_bits;
      const uint8_t* new_data = data + step * full_bytes + (overflow_bits / 8);
      uint8_t new_offset = offset + (overflow_bits % 8);
      if (new_offset >= 8) {
        new_offset -= 8;
        ++new_data;
      }
      return PackedBitsIteratorImpl{new_data, new_offset};
    }
  }

  constexpr PackedBitsIteratorImpl operator-(const size_t& step) const {
    if constexpr (is_byte_aligned) {
      return PackedBitsIteratorImpl<bits>{data - step * full_bytes};
    } else {
      const size_t underflow_bits = step * extra_bits;
      const uint8_t* new_data = data - step * full_bytes - (underflow_bits / 8);
      uint8_t new_offset = offset - (underflow_bits % 8);
      if (new_offset >= 8) {  // if new_offset underflow-ed
        new_offset += 8;      // re-overflow to within [0, 8)
        --new_data;
      }
      return PackedBitsIteratorImpl{new_data, new_offset};
    }
  }

  constexpr PackedBitsIteratorImpl& operator+=(const size_t& step) {
    (*this) = (*this) + step;
    return *this;
  }

  constexpr PackedBitsIteratorImpl& operator-=(const size_t& step) {
    (*this) = (*this) - step;
    return *this;
  }

  constexpr auto operator<=>(const PackedBitsIteratorImpl&) const = default;

  constexpr difference_type operator-(
      const PackedBitsIteratorImpl& other) const {
    if constexpr (is_byte_aligned) {
      const difference_type difference = data - other.data;
      assert(difference % full_bytes == 0);
      return difference / full_bytes;
    } else {
      const difference_type difference =
          8 * (data - other.data) + offset - other.offset;
      assert(difference % bits == 0);
      return difference / bits;
    }
  }

  constexpr value_type operator[](const size_t& i) { return *((*this) + i); }
};

template <uint8_t bits>
class PackedBitsIterator : public PackedBitsIteratorImpl<bits, false> {
 public:
  constexpr auto operator*() {
    return PackedBitsReference<bits>{this->data, this->offset};
  }
};

template <uint8_t bits>
class PackedBitsConstIterator : public PackedBitsIteratorImpl<bits, true> {
  constexpr get_smallest_unsigned_int_t<bits> operator*() const {
    return PackedBitsReference<bits>{this->data, this->offset};
  }
};

template <uint8_t bits, size_t n>
class PackedBitsArray {
  static_assert(bits > 0 && bits <= 64);

 private:
  static constexpr bool is_byte_aligned = bits % 8 == 0;
  static constexpr uint8_t full_bytes = bits / 8;
  static constexpr uint8_t extra_bits = bits % 8;

  std::array<uint8_t, (n * bits + 7) / 8> data;  // add 7 to round up

 public:
  using size_type = size_t;
  using reference = PackedBitsReference<bits>;
  using const_reference = get_smallest_unsigned_int_t<bits>;
  using iterator = PackedBitsIteratorImpl<bits>;
  using const_iterator = PackedBitsConstIterator<bits>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr size_type size() const { return n; }

  constexpr size_type max_size() const { return n; }

  constexpr bool empty() const { return n == 0; }

  constexpr reference operator[](const size_type& i) {
    assert(i < n);
    if constexpr (is_byte_aligned)
      return reference{&data[i * full_bytes]};
    else {
      const size_t overflow_bits = i * extra_bits;
      return reference{&data[i * full_bytes + (overflow_bits / 8)],
                       static_cast<uint8_t>(overflow_bits % 8)};
    }
  };

  constexpr const_reference operator[](const size_type& i) const {
    assert(i < n);
    if constexpr (is_byte_aligned)
      return static_cast<const_reference>(reference{&data[i * full_bytes]});
    else {
      const size_t overflow_bits = i * extra_bits;
      return static_cast<const_reference>(reference{
          &data[i * full_bytes + (overflow_bits / 8)], overflow_bits % 8});
    }
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
    return (*this)[0];
  }

  constexpr const_reference front() const {
    assert(n >= 1);
    return (*this)[0];
  }

  constexpr reference back() {
    assert(n >= 1);
    return (*this)[n - 1];
  }

  constexpr const_reference back() const {
    assert(n >= 1);
    return (*this)[n - 1];
  }

  constexpr iterator begin() {
    if constexpr (is_byte_aligned)
      return iterator{data.begin()};
    else
      return iterator{data.begin(), 0};
  }

  constexpr iterator end() {
    if constexpr (is_byte_aligned)
      return iterator{data.end()};
    else {
      /** static **/ constexpr size_t overflow_bits = n * extra_bits;
      return iterator{&data[n * full_bytes + (overflow_bits / 8)],
                      overflow_bits % 8};
    }
  }

  constexpr const_iterator begin() const {
    if constexpr (is_byte_aligned)
      return iterator{data.cbegin()};
    else
      return iterator{data.cbegin(), 0};
  }

  constexpr const_iterator end() const {
    if constexpr (is_byte_aligned)
      return iterator{data.cend()};
    else {
      /** static **/ constexpr size_t overflow_bits = n * extra_bits;
      return iterator{&data[n * full_bytes + (overflow_bits / 8)],
                      overflow_bits % 8};
    }
  }

  constexpr const_iterator cbegin() const {
    if constexpr (is_byte_aligned)
      return iterator{data.cbegin()};
    else
      return iterator{data.cbegin(), 0};
  }

  constexpr const_iterator cend() const {
    if constexpr (is_byte_aligned)
      return iterator{data.cend()};
    else {
      /** static **/ constexpr size_t overflow_bits = n * extra_bits;
      return iterator{&data[n * full_bytes + (overflow_bits / 8)],
                      overflow_bits % 8};
    }
  }

  constexpr reverse_iterator rbegin() {
    if constexpr (is_byte_aligned)
      return reverse_iterator{data.end()};
    else {
      /** static **/ constexpr size_t overflow_bits = n * extra_bits;
      return reverse_iterator{&data[n * full_bytes + (overflow_bits / 8)],
                              overflow_bits % 8};
    }
  }

  constexpr reverse_iterator rend() {
    if constexpr (is_byte_aligned)
      return reverse_iterator{data.rend()};
    else
      return reverse_iterator{data.rend(), 0};
  }

  constexpr const_reverse_iterator rbegin() const {
    if constexpr (is_byte_aligned)
      return reverse_iterator{data.cend()};
    else {
      /** static **/ constexpr size_t overflow_bits = n * extra_bits;
      return reverse_iterator{&data[n * full_bytes + (overflow_bits / 8)],
                              overflow_bits % 8};
    }
  }

  constexpr const_reverse_iterator rend() const {
    if constexpr (is_byte_aligned)
      return reverse_iterator{data.crend()};
    else
      return reverse_iterator{data.crend(), 0};
  }

  constexpr const_reverse_iterator crbegin() const {
    if constexpr (is_byte_aligned)
      return reverse_iterator{data.cend()};
    else {
      /** static **/ constexpr size_t overflow_bits = n * extra_bits;
      return reverse_iterator{&data[n * full_bytes + (overflow_bits / 8)],
                              overflow_bits % 8};
    }
  }

  constexpr const_reverse_iterator crend() const {
    if constexpr (is_byte_aligned)
      return reverse_iterator{data.crend()};
    else
      return reverse_iterator{data.crend(), 0};
  }

  constexpr void fill(const const_reference& value) {
    for (auto& element : *this) element = value;
  }
};

template <size_t n>
class PackedBitsArray<8, n> : public std::array<uint8_t, n> {};
template <size_t n>
class PackedBitsArray<16, n> : public std::array<uint16_t, n> {};
template <size_t n>
class PackedBitsArray<32, n> : public std::array<uint32_t, n> {};
template <size_t n>
class PackedBitsArray<64, n> : public std::array<uint64_t, n> {};
}  // namespace utility
