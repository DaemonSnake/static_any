#pragma once

#include <optional>

#include "static_any.hpp"
#include "unconstexpr/unique_id.hpp"

namespace Result {

using StaticAny::static_any;
using StaticAny::visit;

template <class T>
struct raise_item {
  T data;
};

template <class T, unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
struct result {
  using any = static_any<Id>;

  std::optional<T> res;
  any error{};

  constexpr result() = default;

  template <class V>
  constexpr result(raise_item<V>&& obj) : error{std::move(obj.data)} {}

  template <class V>
  constexpr result(V&& item) : res{std::forward<V>(item)} {}

  constexpr result(result&& other) = default;

  template <unconstexpr::id_value OId>
  constexpr result(result<T, OId>&& other)
      : res{std::move(other.res)}, error{std::move(other.error)} {}

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
  constexpr decltype(auto) operator*() const { return *res; }
  constexpr decltype(auto) operator*() { return *res; }
};

template <class T>
constexpr auto raise(T&& item) -> raise_item<std::decay_t<T>> {
  return {std::forward<T>(item)};
}

}  // namespace Result