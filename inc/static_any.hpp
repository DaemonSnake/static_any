#pragma once

#include <type_traits>
#include <utility>

#include "common_ret.hpp"
#include "globals.hpp"
#include "index_convert.hpp"
#include "type_list.hpp"
#include "unconstexpr/unconstexpr.hpp"
#include "visit.hpp"

namespace StaticAny {

namespace Details {
template <class Self, class Other>
constexpr bool transfer_copy() {
  return []<class... Args>(TypeList::type_list<Args...> const *) {
    return (noexcept(Self{std::declval<Args>()}) && ...);
  }
  (*Other::all_types);
}

template <class... SArgs, class... OArgs, class Other>
constexpr std::size_t convert_index(TypeList::type_list<SArgs...> const *n_list,
                               TypeList::type_list<OArgs...> const *o_list,
                               Other const &o) {
  constexpr auto find_idxs =
      std::index_sequence<IndexConvert::find<OArgs, SArgs...>()...>{};
  return IndexConvert::convert_index_impl(find_idxs, *n_list, *o_list,
                                          o.get_index());
}

}  // namespace Details

//static_any is declared in an anonymous namespace
//the reason is to make sure that it is only used in the same translation unit
namespace {
template <unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
class static_any {
  std::size_t index = not_found;
  Repr::any_base *ptr = {};

  template <unconstexpr::id_value>
  friend class static_any;

 public:
  constexpr std::size_t get_index() const { return index; }
  constexpr const Repr::any_base *get_ptr() const { return ptr; }
  constexpr Repr::any_base *get_ptr() { return ptr; }

  static constexpr unconstexpr::meta_value<
      static_cast<TypeList::type_list<> *>(nullptr), 1, Id>
      all_types{};

  template <class T>
  constexpr static_any(T &&item) noexcept(
      noexcept(all_types
               << ret_lambda<std::remove_pointer_t<decltype(*all_types)>, T>) ||
      true)
      : index{std::remove_pointer_t<decltype(
            *all_types)>::template get_index<T>()},
        ptr{new Repr::any_impl_t<T>{std::forward<T>(item)}} {}

  //Copies types of other static_any into this one
  // steal its pointer and convert its index
  template <unconstexpr::id_value OId>
  constexpr static_any(static_any<OId> &&other) noexcept(
      Details::transfer_copy<static_any, static_any<OId>>() || true)
      : index{Details::convert_index(*all_types, *static_any<OId>::all_types,
                                     other)},
        ptr{other.ptr} {
    other.ptr = nullptr;
    other.index = not_found;
  }

  constexpr void release() {
    if (ptr) {
      delete ptr;
      ptr = nullptr;
    }
    index = not_found;
  }

  constexpr operator bool() const { return index != not_found; }

  constexpr static_any() = default;
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

  template <unconstexpr::id_value OId>
  constexpr static_any(static_any<OId> const &other) = delete;
  template <unconstexpr::id_value OId>
  constexpr static_any &operator=(static_any<OId> const &other) = delete;
};
}

using Visit::visit;

}  // namespace StaticAny