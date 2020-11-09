#pragma once

#include <type_traits>
#include <utility>

#include "type_traits.hpp"

namespace StaticAny::Repr {

struct any_base {
  constexpr virtual ~any_base() = default;
  constexpr virtual any_base *clone() const = 0;
};

template <class T>
class any_impl : public any_base {
  T data;

 public:
  template <class Tcv>
  constexpr any_impl(Tcv &&data) : data{std::forward<Tcv>(data)} {}
  any_base *clone() const { return new any_impl{data}; }
  constexpr decltype(auto) get() const { return data; }
  constexpr decltype(auto) get() { return data; }
};

template <class T>
using any_impl_t = any_impl<std::decay_t<T>>;

template <class Self, class T>
using any_ptr_t =
    Traits::if_add_t<std::is_const, Self, std::add_const, any_impl_t<T> *>;

}  // namespace StaticAny::Repr