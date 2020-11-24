#pragma once

#include <type_traits>
#include <utility>

#include "type_traits.hpp"
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
  return
      []<class... Args>(TypeList::type_list<Args...> const * /*current_list*/) {
    return (noexcept(Self{std::declval<Args>()}) && ...);
  }
  (*Other::all_types);
}

template <class... SArgs, class... OArgs, class Other>
constexpr size_t convert_index(TypeList::type_list<SArgs...> const *n_list,
                               TypeList::type_list<OArgs...> const *o_list,
                               Other const &o) {
  constexpr auto find_idxs =
      std::index_sequence<IndexConvert::find<OArgs, SArgs...>()...>{};
  return IndexConvert::convert_index_impl(find_idxs, *n_list, *o_list,
                                          o.get_index());
}

struct static_any_base {};

}  // namespace Details


// static_any is declared in an anonymous namespace on purpose
// the reason is to make sure that any static_any<> is only used in the same translation unit
namespace {  // NOLINT(cert-dcl59-cpp,google-build-namespaces)
template <unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
class static_any : public Details::static_any_base {
  size_t index = not_found;
  Repr::any_base *ptr = {};

  template <unconstexpr::id_value>
  friend class static_any;

 public:
  [[nodiscard]] constexpr size_t get_index() const { return index; }
  [[nodiscard]] constexpr const Repr::any_base *get_ptr() const { return ptr; }
  [[nodiscard]] constexpr Repr::any_base *get_ptr() { return ptr; }

  static constexpr unconstexpr::meta_value<
      static_cast<TypeList::type_list<> *>(nullptr), 1, Id>
      all_types{};

  template <Traits::not_derived<Details::static_any_base> T>
  constexpr static_any(T &&item) noexcept(noexcept(
      all_types << ret_lambda<std::remove_pointer_t<decltype(*all_types)>, T>))
      : index{std::remove_pointer_t<decltype(
            *all_types)>::template get_index<T>()},
        ptr{new Repr::any_impl_t<T>{std::forward<T>(item)}} {
        }

  // Copies types of other static_any into this one
  // steal its pointer and convert its index
  template <unconstexpr::id_value OId>
  constexpr static_any(static_any<OId> &&other) noexcept(
      Details::transfer_copy<static_any, static_any<OId>>())
      : index{Details::convert_index(*all_types, *static_any<OId>::all_types,
                                     other)},
        ptr{other.ptr} {
    other.ptr = nullptr;
    other.index = not_found;
  }

  constexpr void release() {
    if (ptr != nullptr) {
      delete ptr;
      ptr = nullptr;
    }
    index = not_found;
  }

  constexpr operator bool() const { return index != not_found; }

  constexpr static_any() = default;
  constexpr ~static_any() { release(); }

  constexpr static_any(static_any &&other) noexcept
      : index{other.index}, ptr{other.ptr} {
    other.index = not_found;
    other.ptr = nullptr;
  }

  constexpr static_any &operator=(static_any &&other) noexcept {
    release();
    index = other.index;
    ptr = other.ptr;
    other.index = not_found;
    other.ptr = nullptr;
    return *this;
  }

  constexpr static_any(static_any const &other)
      : index{other.index}, ptr{other.ptr->clone()} {
      }

  constexpr static_any &operator=(static_any const &other) noexcept {
    if (&other != this) {
      release();
      index = other.index;
      ptr = other.ptr->clone();
    }
    return *this;
  }

  template <unconstexpr::id_value OId>
  constexpr static_any(static_any<OId> const &other) = delete;
  template <unconstexpr::id_value OId>
  constexpr static_any &operator=(static_any<OId> const &other) = delete;
};
}  // namespace

using Visit::visit; //NOLINT(misc-unused-using-decls)

}  // namespace StaticAny