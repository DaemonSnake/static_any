// Copyright (c) 2020 Bastien Penavayre
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <tuple>
#include <type_traits>

#include "common_ret.hpp"
#include "globals.hpp"

namespace StaticAny::TypeList {

template <class... Args>
struct type_list {
  static constexpr size_t length = sizeof...(Args);

  using tuple = std::tuple<Args...>;

  template <class T>
  static constexpr size_t get_index() {
    size_t i = 0;
    using type = std::decay_t<T>;
    static_cast<void>(((std::is_same_v<Args, type> || (++i, false)) || ...));
    return (i >= sizeof...(Args) ? not_found : i);
  }

  template <class T>
  static constexpr type_list<Args..., T> *push_back() {
    return nullptr;
  }

  template <class Fn, class Self>
  using ret = Return::return_type<Fn, Self, Args...>;

  template <size_t I>
  using get = std::tuple_element_t<I, tuple>;
};

}  // namespace StaticAny::TypeList