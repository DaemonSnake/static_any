#pragma once
#include <optional>
#include <type_traits>
#include <utility>
#include <initializer_list>
#include <tuple>
#include <array>
#include <cstddef>
namespace StaticAny::Traits {

template <template <class> class Check, class For, template <class> class Adder,
          class On>
struct if_add {
  using type =
      std::conditional_t<Check<For>::value, typename Adder<On>::type, On>;
};

template <template <class> class Check, class For, template <class> class Adder,
          class On>
using if_add_t = typename if_add<Check, For, Adder, On>::type;

template <size_t I, auto... Is>
static constexpr auto get_n = std::tuple_element_t<
    I, std::tuple<std::integral_constant<decltype(Is), Is>...>>::value;

template<class Derived, class Base>
concept derived = std::is_base_of_v<Base, Derived>;

template<class Derived, class Base>
concept not_derived = !derived<Derived, Base>;

}
namespace StaticAny::Return {

namespace details {

using StaticAny::Traits::if_add_t;

struct convertible {
  template <class T>
  constexpr explicit operator T() const noexcept;
};

template <class T>
struct common_utility {
  using type = std::conditional_t<std::is_void_v<T>, convertible, T>;
};

template <class... Args>
struct common {
  using ret_type = std::common_type_t<typename common_utility<Args>::type...>;
  using type = std::conditional_t<std::is_same_v<convertible, ret_type>, bool,
                                  std::optional<ret_type>>;
};

template <>
struct common<> {
  using type = bool;
};

template <class Fn, class Arg, class Self>
struct result_of {
  using larg = std::add_lvalue_reference_t<Arg>;
  using cv_larg = if_add_t<std::is_const, Self, std::add_const, larg>;
  using type = std::result_of_t<Fn(cv_larg)>;
};

}

template <class Fn, class Self, class... Args>
using return_type = typename details::common<
    typename details::result_of<Fn, Args, Self>::type...>::type;

}
namespace StaticAny {
static constexpr size_t not_found = -1UL;

template <class Old, class T>
inline constexpr auto ret_lambda =
    [] { return Old::template push_back<std::decay_t<T>>(); };
}
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
  switch (index) {
    case I + (0): { constexpr size_t n = I + (0); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (1): { constexpr size_t n = I + (1); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (2): { constexpr size_t n = I + (2); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (3): { constexpr size_t n = I + (3); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (4): { constexpr size_t n = I + (4); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (5): { constexpr size_t n = I + (5); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (6): { constexpr size_t n = I + (6); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (7): { constexpr size_t n = I + (7); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (8): { constexpr size_t n = I + (8); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (9): { constexpr size_t n = I + (9); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (10): { constexpr size_t n = I + (10); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (11): { constexpr size_t n = I + (11); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (12): { constexpr size_t n = I + (12); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (13): { constexpr size_t n = I + (13); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (14): { constexpr size_t n = I + (14); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
    case I + (15): { constexpr size_t n = I + (15); if constexpr (n >= sizeof...(OArgs)) return not_found; else return Traits::get_n<n, FoundIdxs...>; };
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

}
namespace StaticAny::TypeList {

template <class... Args>
struct type_list {
  static constexpr size_t length = sizeof...(Args);

  using tuple = std::tuple<Args...>;

  template <class T>
  static constexpr size_t get_index() {
    size_t i = 0;
    using type = std::decay_t<T>;
    static_cast<void>(((std::is_same_v<Args, type> || (++i, false)) || ...));
    return (i >= sizeof...(Args) ? not_found : i);
  }

  template <class T>
  static constexpr type_list<Args..., T> *push_back() {
    return nullptr;
  }

  template <class Fn, class Self>
  using ret = Return::return_type<Fn, Self, Args...>;

  template <size_t I>
  using get = std::tuple_element_t<I, tuple>;
};

}

namespace unconstexpr {

using id_value = const int *;

template <id_value>
struct id_t {};

template <class T>
struct unique_id {
  static constexpr int value = 0;
  constexpr unique_id(T const &) {}
  constexpr operator id_value() const { return &value; }
};
}

namespace unconstexpr {
template <auto V>
struct value_t {
  static constexpr auto value = V;
};

template <class Lambda>
struct value_lambda {
  static constexpr auto value = Lambda{}();
};
}

namespace unconstexpr {

template <class Meta, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto current_index(Meta const& = {}) {
  constexpr auto idx = Meta::template last_index<Id>();
  return idx;
}

template <class Meta, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto current_value(Meta const& = {}) {
  constexpr auto idx = current_index<Meta, Id>();
  return Meta::template value<Id, idx>();
}

namespace increment_hack {

template <class Meta, class ValueInc, id_value Id>
struct increment_info {
  static constexpr auto idx = current_index<Meta, Id>();
  static constexpr auto value = Meta::template value<Id, idx>();
  static constexpr auto nvalue = value + ValueInc::value;
};
}

template <class Meta, bool postincrement = false,
          class ValueInc = value_t<Meta::increment>, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto increment(Meta const& = {}) {
  using hack = increment_hack::template increment_info<Meta, ValueInc, Id>;
  constexpr auto idx = hack::idx;
  constexpr auto value = hack::value;
  constexpr auto nvalue = hack::nvalue;
  constexpr auto nidx = idx + Meta::increment;
  constexpr auto lambda_next = [] { return hack::nvalue; };
  using value_holder = value_lambda<decltype(lambda_next)>;
  static_assert(Meta::template set_value<nidx, value_holder>);
  if constexpr (postincrement)
    return value;
  else
    return nvalue;
}

template <class Meta, auto Value, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto set(Meta const& = {}, value_t<Value> const& = {}) {
  constexpr auto idx = current_index<Meta, Id>();
  constexpr auto nidx = idx + Meta::increment;
  using holder = value_t<Value>;
  static_assert(Meta::template set_value<nidx, holder>);
  return Value;
}

template <class Meta, class Lambda, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto set(Meta const& = {}, Lambda const& = {}) {
  constexpr auto idx = current_index<Meta, Id>();
  constexpr auto nidx = idx + Meta::increment;
  using holder = value_lambda<Lambda>;
  static_assert(Meta::template set_value<nidx, holder>);
  return holder::value;
}

}
namespace unconstexpr {

template <auto Init = 0, auto Inc = 1, id_value = unique_id([] {})>
struct meta_value {
  template <auto Index>
  struct flagCheck {
    template <id_value Id>
    friend constexpr auto unconstexpr_adl(flagCheck, id_t<Id> const &);
  };

  template <auto Index>
  struct flagGet {
    template <id_value Id>
    friend constexpr auto unconstexpr_adl(flagGet, id_t<Id> const &);
  };

  template <auto Index, class ValueHolder = value_t<Index>>
  struct writer {
    template <id_value Id>
    friend constexpr auto unconstexpr_adl(flagCheck<Index>, id_t<Id> const &) {
      return true;
    }

    template <id_value Id>
    friend constexpr auto unconstexpr_adl(flagGet<Index>, id_t<Id> const &) {
      return ValueHolder::value;
    }
  };

  static_assert(sizeof(writer<0, value_t<Init>>));

  template <auto Index, class T, class = decltype(unconstexpr_adl(flagCheck<Index>{}, T{}))>
  static constexpr bool exists(int) {
    return true;
  }

  template <int, class>
  static constexpr bool exists(float) {
    return false;
  }

  template <id_value Id, auto Index = 0>
  static constexpr auto last_index() {
    using unique_type = id_t<Id>;
    constexpr bool ok = exists<Index + Inc, unique_type>(int{});
    if constexpr (ok)
      return last_index<Id, Index + Inc>();
    else
      return Index;
  }

  template <id_value Id, auto Index>
  static constexpr auto value() {
    using unique_type = id_t<Id>;
    return unconstexpr_adl(flagGet<Index>{}, unique_type{});
  }

  static constexpr bool is_meta_var = true;
  static constexpr auto increment = Inc;

  template <auto Index, class ValueHolder>
  static constexpr bool set_value = sizeof(writer<Index, ValueHolder>);
};

}
namespace unconstexpr {

template <class Meta, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator*(Meta const &) {
  return current_value<Meta, Id>();
}

template <class Meta, class... Opt, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator++(Meta const &, Opt...) {
  constexpr bool is_postincrement = sizeof...(Opt) != 0;
  using inc = value_t<Meta::increment>;
  return increment<Meta, is_postincrement, inc, Id>();
}

template <class Meta, class... Opt, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator--(Meta const &, Opt...) {
  constexpr bool is_postincrement = sizeof...(Opt) != 0;
  using inc = value_t<-1 * Meta::increment>;
  return increment<Meta, is_postincrement, inc, Id>();
}

template <class Meta, auto Value, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator+=(Meta const &c, value_t<Value> const &) {
  static_assert(sizeof(increment<Meta, false, value_t<Value>, Id>()));
  return c;
}

template <class Meta, class Lambda, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator+=(Meta const &c, Lambda const &) {
  static_assert(sizeof(increment<Meta, false, value_lambda<Lambda>, Id>()));
  return c;
}

template <class Meta, auto Value, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator<<(Meta const &c, value_t<Value> const &) {
  static_assert(sizeof(set<Meta, Value, Id>()));
  return c;
}

template <class Meta, class Holder, id_value Id = unique_id([] {}), bool = Meta::is_meta_var>
constexpr auto operator<<(Meta const &c, Holder const &) {
  static_assert(sizeof(set<Meta, Holder, Id>()));
  return c;
}

}
namespace StaticAny::Repr {

struct any_base {
  constexpr any_base() = default;
  constexpr virtual ~any_base() = default;
  [[nodiscard]] constexpr virtual any_base *clone() const = 0;

  constexpr any_base(any_base const &) = delete;
  constexpr any_base(any_base &&) = delete;
  constexpr any_base &operator=(any_base const &) = delete;
  constexpr any_base &operator=(any_base &&) = delete;
};

template <class T>
class any_impl : public any_base {
  T data;

 public:
  template <class Tcv>
  explicit constexpr any_impl(Tcv &&data) : data{std::forward<Tcv>(data)} {}
  [[nodiscard]] any_base *clone() const override { return new any_impl{data}; }
  constexpr decltype(auto) get() const { return data; }
  constexpr decltype(auto) get() { return data; }
  constexpr ~any_impl() override = default;

  constexpr any_impl() = delete;
  constexpr any_impl(any_impl const &) = delete;
  constexpr any_impl(any_impl &&) = delete;
  constexpr any_impl &operator=(any_impl const &) = delete;
  constexpr any_impl &operator=(any_impl &&) = delete;
};

template <class T>
using any_impl_t = any_impl<std::decay_t<T>>;

template <class Self, class T>
using any_ptr_t =
    Traits::if_add_t<std::is_const, Self, std::add_const, any_impl_t<T> *>;

}

namespace StaticAny::Visit {

template <class Self, class Fn, unconstexpr::id_value >
struct visit_info {
  using self = std::remove_reference_t<std::remove_cv_t<Self>>;
  using type_list = typename std::remove_pointer_t<decltype(*self::all_types)>;
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
  if constexpr (Info::is_bool) {
    return (visitor(ptr->get()), true);
  } else {
    return typename Info::ret{visitor(ptr->get())};
  }
};

template <class info, size_t I = 0, class Self, class Fn>
constexpr decltype(auto) visit_helper(Self *self, Fn &&visitor) {
  switch (self->get_index()) {
    case I + (0): { constexpr size_t n = I + (0); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (1): { constexpr size_t n = I + (1); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (2): { constexpr size_t n = I + (2); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (3): { constexpr size_t n = I + (3); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (4): { constexpr size_t n = I + (4); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (5): { constexpr size_t n = I + (5); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (6): { constexpr size_t n = I + (6); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (7): { constexpr size_t n = I + (7); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (8): { constexpr size_t n = I + (8); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (9): { constexpr size_t n = I + (9); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (10): { constexpr size_t n = I + (10); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (11): { constexpr size_t n = I + (11); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (12): { constexpr size_t n = I + (12); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (13): { constexpr size_t n = I + (13); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (14): { constexpr size_t n = I + (14); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };
    case I + (15): { constexpr size_t n = I + (15); if constexpr (n >= info::type_list::length) return typename info::ret{}; else return visit_at<info, n>(self, std::forward<Fn>(visitor)); };

    default: {
      constexpr size_t n = 16 + I;
      if constexpr (n >= info::type_list::length) {
        return typename info::ret{};
      } else {
        return visit_helper<info, n>(self, std::forward<Fn>(visitor));
      }
    }
  }

}

template <unconstexpr::id_value IdV = unconstexpr::unique_id([] {}), class Self,
          class Fn>
constexpr decltype(auto) visit(Self &&self, Fn &&visitor) noexcept {
  using info = visit_info<Self, Fn, IdV>;
  if (self.get_index() == not_found) {
    return typename info::ret{};
  }
  return visit_helper<info>(&self, std::forward<Fn>(visitor));
}

}

namespace StaticAny {

namespace Details {
template <class Self, class Other>
constexpr bool transfer_copy() {
  return
      []<class... Args>(TypeList::type_list<Args...> const * ) {
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

}

namespace {
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
}

using Visit::visit;

}

namespace Result {

using StaticAny::static_any;
using StaticAny::visit;
using StaticAny::Traits::not_derived;
using StaticAny::Traits::derived;

struct raise_item_base {};

template <class T>
struct raise_item : raise_item_base {
  using raise_item_t = void;
  T data;
};

struct result_base {};

template <class T, unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
class result : public result_base {
  using any = static_any<Id>;

  std::optional<T> res{};
  any error{};

  template <class Ot, unconstexpr::id_value Oid>
  friend class result;

 public:
  static constexpr auto id = Id;
  using type = T;

  constexpr result() = default;
  constexpr ~result() = default;

  template <class V>
  constexpr result(raise_item<V>&& obj) : error{std::move(obj.data)} {}

  template <StaticAny::Traits::not_derived<result_base> V>
  requires(std::is_constructible_v<std::optional<T>, V>) constexpr result(
      V&& item)
      : res{std::forward<V>(item)} {}

  template <derived<any> AnyCv>
  constexpr result(AnyCv&& error) : error{std::forward<AnyCv>(error)} {}

  constexpr result(result&& other) noexcept = default;

  template <unconstexpr::id_value OId>
  constexpr result(result<T, OId>&& other)
      : res{std::move(other.res)}, error{std::move(other.error)} {}

  constexpr result& operator=(result&&) noexcept = default;
  constexpr result& operator=(result const&) = delete;
  constexpr result(result const&) = delete;

  template <class Fn>
  auto catch_except(Fn&& visitor) {
    return visit(error, std::forward<Fn>(visitor));
  }

  template <class Fn>
  auto catch_except(Fn&& visitor) const {
    return visit(error, std::forward<Fn>(visitor));
  }

  template <class Self>
  static constexpr decltype(auto) get(Self& self) {
    return *self.data;
  }

  constexpr operator bool() const { return !error; }
  constexpr decltype(auto) operator*() const& { return *res; }
  constexpr decltype(auto) operator*() & { return *res; }
  constexpr auto operator*() && { return *std::move(res); }

  constexpr operator decltype(auto)() & { return this->operator*(); }
  constexpr operator decltype(auto)() const& { return this->operator*(); }
  constexpr operator auto() && { return this->operator*(); }

  template <derived<result_base> Res, class Fn>
  friend decltype(auto) operator|(Res&& r, Fn&& fn);
};

template<derived<result_base> T>
result(T&&) -> result<typename std::decay_t<T>::type, std::decay_t<T>::id>;

template<class T, unconstexpr::id_value Id = unconstexpr::unique_id([] {})>
result(T&&) -> result<T, Id>;

template <class T>
constexpr auto raise(T&& item) -> raise_item<std::decay_t<T>> {
  return {{}, std::forward<T>(item)};
}

template <derived<result_base> Res, class Fn>
decltype(auto) operator|(Res&& r, Fn&& fn)
{
  using ret = std::result_of_t<Fn(decltype(*r))>;

  if constexpr (std::is_void_v<ret>) {
    if (r) {
      std::forward<Fn>(fn)(*std::forward<Res>(r));
    }
    return std::forward<Res>(r);
  } else if constexpr (requires() { r = std::declval<ret>(); }) {
    if (r) {
      auto tmp = std::forward<Fn>(fn)(*std::forward<Res>(r));
      r = std::move(tmp);
    }
    return std::forward<Res>(r);
  } else {
    using result_t = result<ret, std::decay_t<Res>::id>;
    if (r) {
      return result_t{std::forward<Fn>(fn)(*std::forward<Res>(r))};
    }
    return result_t{std::forward<Res>(r).error};
  }
}

}
