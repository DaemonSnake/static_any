// Copyright (c) 2020 Bastien Penavayre
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <optional>

#include "type_traits.hpp"

namespace StaticAny::Return {

namespace details {

using StaticAny::Traits::if_add_t;

struct convertible {
  template <class T>
  constexpr explicit operator T() const noexcept;
};

template <class T>
struct common_utility {
  using type = std::conditional_t<std::is_void_v<T>, convertible, T>;
};

template <class... Args>
struct common {
  using ret_type = std::common_type_t<typename common_utility<Args>::type...>;
  using type = std::conditional_t<std::is_same_v<convertible, ret_type>, bool,
                                  std::optional<ret_type>>;
};

template <>
struct common<> {
  using type = bool;
};

template <class Fn, class Arg, class Self>
struct result_of {
  using larg = std::add_lvalue_reference_t<Arg>;
  using cv_larg = if_add_t<std::is_const, Self, std::add_const, larg>;
  using type = std::result_of_t<Fn(cv_larg)>;
};

}  // namespace details

template <class Fn, class Self, class... Args>
using return_type = typename details::common<
    typename details::result_of<Fn, Args, Self>::type...>::type;

}  // namespace StaticAny::Return