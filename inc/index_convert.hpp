// Copyright (c) 2020 Bastien Penavayre
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <tuple>
#include <utility>

#include "globals.hpp"
#include "type_traits.hpp"

namespace StaticAny::IndexConvert {

template <class T, class... Args>
constexpr size_t find() {
  size_t i = 0;
  static_cast<void>(((std::is_same_v<T, Args> || (++i, false)) || ...));
  return i >= sizeof...(Args) ? not_found : i;
}

template <size_t I = 0, size_t... FoundIdxs, template <class...> class H,
          class... SArgs, class... OArgs>
constexpr size_t convert_index_impl(std::index_sequence<FoundIdxs...> idxs,
                                    H<SArgs...> const &n, H<OArgs...> const &o,
                                    size_t index) {
#ifdef _VISIT_CASE
#warning "_VISIT_CASE was already defined"
#undef _VISIT_CASE
#endif
// Saddly haven't found a cleaner way than using macros here.
// Single if+recursion doesn't seem to be well optimized by GCC
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define _VISIT_CASE(N)                       \
  case I + (N): {                            \
    constexpr size_t n = I + (N);            \
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
      constexpr size_t n_cases = 16;
      constexpr size_t n_index = I + n_cases;
      if constexpr (n_index >= sizeof...(OArgs)) {
        return not_found;
      } else {
        return convert_index_impl<n_index>(idxs, n, o);
      }
    }
  }
}

#undef _VISIT_CASE
}  // namespace StaticAny::IndexConvert
