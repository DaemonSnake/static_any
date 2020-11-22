#pragma once

#include <tuple>
#include <utility>

#include "type_traits.hpp"
#include "globals.hpp"

namespace StaticAny::IndexConvert {

template <class T, class... Args>
constexpr std::size_t find() {
  std::size_t i = 0;
  ((std::is_same_v<T, Args> || (++i, false)) || ...);
  return i >= sizeof...(Args) ? not_found : i;
}

template <std::size_t I = 0, std::size_t... FoundIdxs, template <class...> class H,
          class... SArgs, class... OArgs>
constexpr std::size_t convert_index_impl(std::index_sequence<FoundIdxs...> idxs,
                                    H<SArgs...> const &n, H<OArgs...> const &o,
                                    std::size_t index) {
#ifdef _VISIT_CASE
#warning "_VISIT_CASE was already defined"
#undef _VISIT_CASE
#endif
#define _VISIT_CASE(N)                       \
  case I + N: {                              \
    constexpr std::size_t n = I + N;         \
    if constexpr (n >= sizeof...(OArgs))     \
      return not_found;                      \
    else                                     \
      return Traits::get_n<n, FoundIdxs...>; \
  }

  switch (index) {
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
      if constexpr (I + 16 >= sizeof...(OArgs))
        return not_found;
      else
        return convert_index_impl<I + 16>(idxs, n, o);
    }
  }
}

#undef _VISIT_CASE
}  // namespace StaticAny::IndexConvert
