#pragma once

#include <cstddef>
#include <type_traits>

namespace StaticAny {
static constexpr std::size_t not_found = -1ul;

template <class Old, class T>
constexpr auto ret_lambda =
    [] { return Old::template push_back<std::decay_t<T>>(); };
}  // namespace StaticAny
