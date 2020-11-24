// Copyright (c) 2020 Bastien Penavayre
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <optional>

#include "static_any.hpp"
#include "type_traits.hpp"
#include "unconstexpr/unique_id.hpp"

namespace Result {

using StaticAny::static_any;
using StaticAny::visit;
using StaticAny::Traits::not_derived;

struct raise_item_base {};

template <class T>
struct raise_item : raise_item_base {
  using raise_item_t = void;
  T data;
};

struct result_base {};

template <class T, unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
class result : public result_base {
  using any = static_any<Id>;

  std::optional<T> res{};
  any error{};

  template <class Ot, unconstexpr::id_value Oid>
  friend class result;

 public:
  static constexpr auto id = Id;
  using type = T;

  constexpr result() = default;
  constexpr ~result() = default;

  template <class V>
  constexpr result(raise_item<V>&& obj) : error{std::move(obj.data)} {}

  template <StaticAny::Traits::not_derived<result_base> V>
  requires(std::is_constructible_v<std::optional<T>, V>) constexpr result(
      V&& item)
      : res{std::forward<V>(item)} {}

  template <std::same_as<any> AnyCv>
  constexpr result(AnyCv&& error) : error{std::forward<AnyCv>(error)} {}

  constexpr result(result&& other) noexcept = default;

  template <unconstexpr::id_value OId>
  constexpr result(result<T, OId>&& other)
      : res{std::move(other.res)}, error{std::move(other.error)} {}

  constexpr result& operator=(result&&) noexcept = default;
  constexpr result& operator=(result const&) = delete;
  constexpr result(result const&) = delete;

  template <class Fn>
  auto catch_except(Fn&& visitor) {
    return visit(error, std::forward<Fn>(visitor));
  }

  template <class Fn>
  auto catch_except(Fn&& visitor) const {
    return visit(error, std::forward<Fn>(visitor));
  }

  template <class Self>
  static constexpr decltype(auto) get(Self& self) {
    return *self.data;
  }

  constexpr operator bool() const { return !error; }
  constexpr decltype(auto) operator*() const& { return *res; }
  constexpr decltype(auto) operator*() & { return *res; }
  constexpr auto operator*() && { return *std::move(res); }

  constexpr operator decltype(auto)() & { return this->operator*(); }
  constexpr operator decltype(auto)() const& { return this->operator*(); }
  constexpr operator auto() && { return this->operator*(); }

  template <std::derived_from<result_base> Res, class Fn>
  friend decltype(auto) operator|(Res&& r, Fn&& fn);
};

template<std::derived_from<result_base> T>
result(T&&) -> result<typename std::decay_t<T>::type, std::decay_t<T>::id>;

template<class T, unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
result(T&&) -> result<T, Id>;

template <class T>
constexpr auto raise(T&& item) -> raise_item<std::decay_t<T>> {
  return {{}, std::forward<T>(item)};
}

template <std::derived_from<result_base> Res, class Fn>
decltype(auto) operator|(Res&& r, Fn&& fn)
{
  using ret = std::result_of_t<Fn(decltype(*r))>;

  if constexpr (std::is_void_v<ret>) {
    if (r) {
      std::forward<Fn>(fn)(*std::forward<Res>(r));
    }
    return std::forward<Res>(r);
  } else if constexpr (requires() { r = std::declval<ret>(); }) {
    if (r) {
      auto tmp = std::forward<Fn>(fn)(*std::forward<Res>(r));
      r = std::move(tmp);
    }
    return std::forward<Res>(r);
  } else {
    using result_t = result<ret, std::decay_t<Res>::id>;
    if (r) {
      return result_t{std::forward<Fn>(fn)(*std::forward<Res>(r))};
    }
    return result_t{std::forward<Res>(r).error};
  }
}

}  // namespace Result