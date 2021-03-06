#pragma once

// Copyright (c) 2020 Bastien Penavayre
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <tuple>
#include <type_traits>

namespace StaticAny::Traits {

template <template <class> class Check, class For, template <class> class Adder,
          class On>
struct if_add {
  using type =
      std::conditional_t<Check<For>::value, typename Adder<On>::type, On>;
};

template <template <class> class Check, class For, template <class> class Adder,
          class On>
using if_add_t = typename if_add<Check, For, Adder, On>::type;

template <size_t I, auto... Is>
static constexpr auto get_n = std::tuple_element_t<
    I, std::tuple<std::integral_constant<decltype(Is), Is>...>>::value;

template<class Derived, class Base>
concept derived = std::is_base_of_v<Base, std::decay_t<Derived>>;

template<class Derived, class Base>
concept not_derived = !derived<Derived, Base>;

}  // namespace StaticAny::Traits
