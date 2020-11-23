#pragma once

#include <cstddef>
#include <type_traits>

namespace StaticAny {
static constexpr size_t not_found = -1UL;

template <class Old, class T>
inline constexpr auto ret_lambda =
    [] { return Old::template push_back<std::decay_t<T>>(); };
}  // namespace StaticAny
