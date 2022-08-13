#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename Key, typename Value, size_t Size>
struct ConstexprBimap {
  std::array<std::pair<Key, Value>, Size> data;

  constexpr const Value& getValue(const Key& key) const {
    const auto it = std::find_if(
        data.begin(), data.end(),
        [&](const std::pair<Key, Value>& pair) { return pair.first == key; });
    if (it == data.end()) throw std::invalid_argument("Could not find key!");
    return it->second;
  }

  constexpr const Key& getKey(const Value& value) const {
    const auto it = std::find_if(data.begin(), data.end(),
                                 [&](const std::pair<Key, Value>& pair) {
                                   return pair.second == value;
                                 });
    if (it == data.end()) throw std::invalid_argument("Could not find value!");
    return it->first;
  }
};
