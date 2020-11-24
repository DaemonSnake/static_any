// Copyright (c) 2020 Bastien Penavayre
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <type_traits>
#include <utility>

#include "type_traits.hpp"

namespace StaticAny::Repr {

struct any_base {
  constexpr any_base() = default;
  constexpr virtual ~any_base() = default;
  [[nodiscard]] constexpr virtual any_base *clone() const = 0;

  constexpr any_base(any_base const &) = delete;
  constexpr any_base(any_base &&) = delete;
  constexpr any_base &operator=(any_base const &) = delete;
  constexpr any_base &operator=(any_base &&) = delete;
};

template <class T>
class any_impl : public any_base {
  T data;

 public:
  template <class Tcv>
  explicit constexpr any_impl(Tcv &&data) : data{std::forward<Tcv>(data)} {}
  [[nodiscard]] any_base *clone() const override { return new any_impl{data}; } //NOLINT(cppcoreguidelines-owning-memory)
  constexpr decltype(auto) get() const { return data; }
  constexpr decltype(auto) get() { return data; }
  constexpr ~any_impl() override = default;

  constexpr any_impl() = delete;
  constexpr any_impl(any_impl const &) = delete;
  constexpr any_impl(any_impl &&) = delete;
  constexpr any_impl &operator=(any_impl const &) = delete;
  constexpr any_impl &operator=(any_impl &&) = delete;
};

template <class T>
using any_impl_t = any_impl<std::decay_t<T>>;

template <class Self, class T>
using any_ptr_t =
    Traits::if_add_t<std::is_const, Self, std::add_const, any_impl_t<T> *>;

}  // namespace StaticAny::Repr