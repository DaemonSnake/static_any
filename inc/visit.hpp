#pragma once

#include <type_traits>
#include <utility>

#include "unconstexpr/unique_id.hpp"
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
};

template <class Info, size_t N, class Self, class Fn,
          class type = typename Info::template get<N>,
          class ptr_t = Repr::any_ptr_t<Self, type>>
constexpr auto visit_at(Self *self, Fn &&visitor) {
  auto ptr = static_cast<ptr_t>(self->get_ptr());
  if constexpr (Info::is_bool)
    return (visitor(ptr->get()), true);
  else
    return typename Info::ret{visitor(ptr->get())};
};

template <class info, size_t I = 0, class Self, class Fn>
constexpr decltype(auto) visit_helper(Self *self, Fn &&visitor)
{
#ifdef _VISIT_CASE
# warning "_VISIT_CASE was already defined"
# undef _VISIT_CASE
#endif
#define _VISIT_CASE(N)                                           \
  case I + N: {                                                  \
    constexpr size_t n = I + N;                                  \
    if constexpr (n >= info::type_list::length)                  \
      return typename info::ret{};                               \
    else                                                         \
      return visit_at<info, n>(self, std::forward<Fn>(visitor)); \
  }

  switch (self->get_index()) {
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
  if (self.get_index() == not_found) return typename info::ret{};
  return visit_helper<info>(&self, std::forward<Fn>(visitor));
}

}  // namespace StaticAny::Visit