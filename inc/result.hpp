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
class result {
  using any = static_any<Id>;

  std::optional<T> res;
  any error{};

  template <class Ot, unconstexpr::id_value Oid>
  friend class result;

 public:
  template<class V>
  using not_same_t = std::enable_if_t<!std::is_same_v<std::decay_t<V>, result>>;

  constexpr result() = default;
  constexpr ~result() = default;

  template <class V>
  constexpr result(raise_item<V>&& obj) : error{std::move(obj.data)} {}

  template <class V, class = not_same_t<V>>
  constexpr result(V&& item) : res{std::forward<V>(item)} {}

  constexpr result(result&& other) noexcept = default;

  template <unconstexpr::id_value OId>
  constexpr explicit result(result<T, OId>&& other)
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
  constexpr decltype(auto) operator*() const & { return *res; }
  constexpr decltype(auto) operator*() & { return *res; }
  constexpr auto operator*() && { return *std::move(res); }

  constexpr operator decltype(auto)() & { return this->operator*(); }
  constexpr operator decltype(auto)() const & { return this->operator*(); }
  constexpr operator auto() && { return this->operator*(); }
};

template <class T>
constexpr auto raise(T&& item) -> raise_item<std::decay_t<T>> {
  return {std::forward<T>(item)};
}

}  // namespace Result