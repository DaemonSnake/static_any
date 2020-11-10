#pragma once

#include <type_traits>
#include <utility>

#include "common_ret.hpp"
#include "globals.hpp"
#include "type_list.hpp"
#include "unconstexpr/unconstexpr.hpp"
#include "visit.hpp"

namespace StaticAny {

template <unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
struct static_any {
  static constexpr unconstexpr::meta_value<
      static_cast<TypeList::type_list<> *>(nullptr), 1, Id>
      all_types{};

  size_t index = not_found;
  Repr::any_base *ptr = {};

  template <class T>
  constexpr static_any(T &&item) noexcept(
      noexcept(all_types
               << ret_lambda<std::remove_pointer_t<decltype(*all_types)>, T>) ||
      true)
      : index{std::remove_pointer_t<decltype(
            *all_types)>::template get_index<T>()},
        ptr{new Repr::any_impl_t<T>{std::forward<T>(item)}} {}

  constexpr void release() {
    if (ptr) {
      delete ptr;
      ptr = nullptr;
    }
    index = not_found;
  }

  constexpr static_any() {}
  constexpr ~static_any() { release(); }

  constexpr static_any(static_any &&other)
      : index{other.index}, ptr{other.ptr} {
    other.index = not_found;
    other.ptr = nullptr;
  }

  constexpr static_any &operator=(static_any &&other) {
    release();
    index = other.index;
    ptr = other.ptr;
    other.index = not_found;
    other.ptr = nullptr;
    return *this;
  }

  constexpr static_any(static_any const &other)
      : index{other.index}, ptr{other.ptr->clone()} {}

  constexpr static_any &operator=(static_any const &other) {
    release();
    index = other.index;
    ptr = other.ptr->clone();
  }
};

using Visit::visit;

}  // namespace StaticAny