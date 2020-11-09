#pragma once

namespace StaticAny {
static constexpr size_t not_found = -1ul;

template <class Old, class T>
constexpr auto ret_lambda =
    [] { return Old::template push_back<std::decay_t<T>>(); };
}  // namespace StaticAny