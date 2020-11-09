#pragma once

#include <type_traits>
#include <utility>

#include "godbolt.hpp"
#include "data_repr.hpp"

namespace StaticAny::Visit {

template <class Self, class Fn, unconstexpr::id_value /*forces reevalution*/>
struct visit_info {
  using self = std::remove_reference_t<std::remove_cv_t<Self>>;
  using type_list =
      typename std::remove_pointer_t<decltype(*self::all_types)>;
  using ret = typename type_list::template ret<Fn, Self>;
  static constexpr bool is_bool = std::is_same_v<bool, ret>;

  template <size_t I>
  using get = typename type_list::template get<I>;

  template <class Ptr>
  static constexpr bool noexcept_value() {
    using fn_ret = decltype(std::declval<Fn>()(*std::declval<Ptr>()));
    bool invoke_noexcept =
        noexcept(std::declval<Fn>()(*std::declval<Ptr>()));
    if constexpr (is_bool)
      return invoke_noexcept;
    else
      return noexcept(ret{std::declval<fn_ret>()}) && invoke_noexcept;
  }
};

template <class Info, size_t N, class Self, class Fn,
          class type = typename Info::template get<N>,
          class ptr_t = Repr::any_ptr_t<Self, type>>
constexpr auto visit_at(Self *self, Fn &&visitor) noexcept(
    Info::template noexcept_value<ptr_t>()) {
  auto ptr = static_cast<ptr_t>(self->ptr);
  if constexpr (Info::is_bool)
    return (visitor(ptr->get()), true);
  else
    return typename Info::ret{visitor(ptr->get())};
};

template <class Info, class Self, class Fn>
constexpr bool noexcept_clause() {
  using self_ptr = std::add_pointer_t<Self>;
  return []<size_t... Is>(std::index_sequence<Is...>) {
    return (noexcept(visit_at<Info, Is>(std::declval<self_ptr>(),
                                        std::declval<Fn>())) &&
            ...);
  }
  (std::make_index_sequence<Info::type_list::length>());
}

template <class info, size_t I = 0, class Self, class Fn>
constexpr decltype(auto) visit_helper(Self *self, Fn &&visitor) {
#define _VISIT_CASE(N)                                           \
  case I + N: {                                                  \
    constexpr size_t n = I + N;                                  \
    if constexpr (n >= info::type_list::length)                  \
      return typename info::ret{};                               \
    else                                                         \
      return visit_at<info, n>(self, std::forward<Fn>(visitor)); \
  }

  switch (self->index) {
    _VISIT_CASE(0);
    _VISIT_CASE(1);
    _VISIT_CASE(2);
    _VISIT_CASE(3);
    _VISIT_CASE(4);
    _VISIT_CASE(5);
    _VISIT_CASE(6);
    _VISIT_CASE(7);
    _VISIT_CASE(8);
    _VISIT_CASE(9);
    _VISIT_CASE(10);
    _VISIT_CASE(11);
    _VISIT_CASE(12);
    _VISIT_CASE(13);
    _VISIT_CASE(14);
    _VISIT_CASE(15);

    default: {
      constexpr size_t n = 16 + I;
      if constexpr (n >= info::type_list::length)
        return typename info::ret{};
      else
        return visit_helper<info, n>(self, std::forward<Fn>(visitor));
    }
  }
#undef _VISIT_CASE
}

template <unconstexpr::id_value IdV = unconstexpr::unique_id([] {}), class Self,
          class Fn>
constexpr decltype(auto) visit(Self &&self, Fn &&visitor) noexcept {
  using info = visit_info<Self, Fn, IdV>;
  if (self.index == not_found) return typename info::ret{};
  if constexpr (!noexcept_clause<info, Self, Fn>()) {
    try {
      return visit_helper<info>(&self, std::forward<Fn>(visitor));
    } catch (...) {
      return typename info::ret{};
    }
  } else
    return visit_helper<info>(&self, std::forward<Fn>(visitor));
}

}  // namespace StaticAny::Visit