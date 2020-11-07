#pragma once

#include "godbolt.hpp"

template<class T> struct type_holder {};
constexpr size_t not_found = -1ul;

template<class... Args> struct idx_type {
    template<size_t = 0, class T> static constexpr size_t get_index(type_holder<T> const &) { return not_found; }
    using tuple = std::tuple<>;
};

template<class Front, class... Tail>
struct idx_type<Front, Tail...> : idx_type<Tail...>
{
    template<size_t I = 0, class T>
    static constexpr size_t get_index(type_holder<T> const &item) { 
        return idx_type<Tail...>::template get_index<I+1>(item);
    }

    template<size_t I = 0>
    static constexpr size_t get_index(type_holder<Front> const &) { return I; }

    using tuple = std::tuple<Front, Tail...>;
};

struct convertible
{
  template<class T>
  constexpr operator T() const noexcept;
};

template<class T>
struct common_utility {
    using type = std::conditional_t<std::is_void_v<T>, convertible, T>;
};

template<class... Args>
struct common {
    using ret_type = std::common_type_t<typename common_utility<Args>::type...>;
    using type = std::conditional_t<
        std::is_same_v<convertible, ret_type>,
        bool,
        std::optional<ret_type>
    >;
};

template<>
struct common<> {
  using type = bool;
};

template<template<class> class Check, class For, template<class> class Adder, class On>
struct if_add
{
    using type = std::conditional_t<
        Check<For>::value,
        typename Adder<On>::type,
        On
    >;
};

template<template<class> class Check, class For, template<class> class Adder, class On>
using if_add_t = typename if_add<Check, For, Adder, On>::type;

template<class Fn, class Arg, class Self>
struct result_of
{
    using larg = std::add_lvalue_reference_t<Arg>;
    using cv_larg = if_add_t<std::is_const, Self, std::add_const, larg>;
    using type = std::result_of_t<Fn(cv_larg)>;
};

struct any_base {
    constexpr virtual ~any_base() = default;
    constexpr virtual any_base *clone() const = 0;
};

template<class T>
struct any_impl : any_base {
    T data;
    template<class Tcv> constexpr any_impl(Tcv&& data): data{std::forward<Tcv>(data)} {}
    any_base *clone() const { return new any_impl{data}; }
};

template<class T>
using any_impl_t = any_impl<std::decay_t<T>>;

template<class Self, class T>
using any_ptr_t = if_add_t<std::is_const, Self, std::add_const, any_impl_t<T> *>;

template<class... Args>
struct type_list : idx_type<Args...>
{
    static constexpr unsigned long length = sizeof...(Args);

    using idx_type<Args...>::get_index;
    using typename idx_type<Args...>::tuple;

    template<class T>
    static constexpr type_list<Args..., T> push_back() { return {}; }

    template<class Fn, class Self>
    using ret = typename common<typename result_of<Fn, Args, Self>::type...>::type;

    template<size_t I>
    using get = std::tuple_element_t<I, typename idx_type<Args...>::tuple>;
};

template<class Old, class T>
constexpr auto ret_lambda = []{ return Old::template push_back<std::decay_t<T>>(); };

template<class Self, class Fn, unconstexpr::id_value IdV>
struct visit_info
{
  using self = std::remove_reference_t<std::remove_cv_t<Self>>;
  using type_list = typename self::template current_type<decltype(*self::all_types)>;
  using ret = typename type_list::template ret<Fn, Self>;
  static constexpr bool is_bool = std::is_same_v<bool, ret>;

  template<size_t I>
  using get = typename type_list::template get<I>;

  template<class Ptr>
  static constexpr bool noexcept_value() {
    using fn_ret = decltype(std::declval<Fn>()(std::declval<Ptr>()->data));
    bool invoke_noexcept = noexcept(std::declval<Fn>()(std::declval<Ptr>()->data));
    if constexpr (is_bool) return invoke_noexcept;
    else return noexcept(ret{std::declval<fn_ret>()}) && invoke_noexcept;
  }
};

template<
  class info, size_t N, class Self, class Fn,
  class type = typename info::template get<N>, class ptr_t = any_ptr_t<Self, type>
>
constexpr auto visit_at(Self *self, Fn &&visitor)
  noexcept(info::template noexcept_value<ptr_t>())
{
    auto ptr = static_cast<ptr_t>(self->ptr);
    if constexpr (info::is_bool) return (visitor(ptr->data), true);
    else return typename info::ret{visitor(ptr->data)};
};

template<class Info, class Self, class Fn>
constexpr bool noexcept_clause()
{
  using self_ptr = std::add_pointer_t<Self>;
  return []<size_t... Is>(std::index_sequence<Is...>) {
    return (noexcept(visit_at<Info, Is>(std::declval<self_ptr>(), std::declval<Fn>())) && ...);
  }(std::make_index_sequence<Info::type_list::length>());
}

template<unconstexpr::id_value IdV = unconstexpr::unique_id([]{}), class Self, class Fn>
constexpr decltype(auto) visit(Self &&self, Fn &&visitor)
{
    using info = visit_info<Self, Fn, IdV>;
    if (self.index == not_found)
        return typename info::ret{};
    if constexpr (!noexcept_clause<info, Self, Fn>()) {
        try { return visit_helper<info>(&self, std::forward<Fn>(visitor)); }
        catch(...) { return typename info::ret{}; }
    }
    else return visit_helper<info>(&self, std::forward<Fn>(visitor));
}

template<class info, size_t I = 0, class Self, class Fn>
constexpr decltype(auto) visit_helper(Self *self, Fn &&visitor)
{
#define _VISIT_CASE(N) \
    case I+N: { \
        constexpr size_t n = I+N; \
        if constexpr (n >= info::type_list::length) return typename info::ret{}; \
        else return visit_at<info, n>(self, std::forward<Fn>(visitor)); \
    }

    switch (self->index) {
        _VISIT_CASE(0); _VISIT_CASE(1); _VISIT_CASE(2); _VISIT_CASE(3);
        _VISIT_CASE(4); _VISIT_CASE(5); _VISIT_CASE(6); _VISIT_CASE(7);
        _VISIT_CASE(8); _VISIT_CASE(9); _VISIT_CASE(10); _VISIT_CASE(11);
        _VISIT_CASE(12); _VISIT_CASE(13); _VISIT_CASE(14); _VISIT_CASE(15);
        
        default: {
            constexpr size_t n = 16+I;
            if constexpr (n >= info::type_list::length) return typename info::ret{};
            else return visit_helper<info, n>(self, std::forward<Fn>(visitor));
        }
    }
#undef _VISIT_CASE

}

template<unconstexpr::id_value Id = unconstexpr::unique_id([]{})>
struct static_any {
    static constexpr unconstexpr::meta_value<static_cast<type_list<>*>(nullptr), 1, Id> all_types{};

    template<class Old, class T>
    constexpr void append() noexcept(noexcept(all_types << ret_lambda<Old, T>) || true) {}

    size_t index = not_found;
    any_base *ptr = {};

    template<class T>
    using current_type = std::remove_pointer_t<T>;

    template<class T>
    constexpr static_any(T&& item) noexcept(noexcept(append<current_type<decltype(*all_types)>, T>()) || true):
        index{current_type<decltype(*all_types)>::get_index(type_holder<std::decay_t<T>>{})},
        ptr{new any_impl_t<T>{std::forward<T>(item)}}
    {
    }

    constexpr void release() {
        if (ptr) {
            delete ptr;
            ptr = nullptr;
        }
        index = not_found;
    }

    constexpr static_any() {}
    constexpr ~static_any() {
        release();
    }

    constexpr static_any(static_any &&other):
        index{other.index},
        ptr{other.ptr}
    {
        other.index = not_found;
        other.ptr = nullptr;
    }

    constexpr static_any &operator=(static_any &&other)
    {
        release();
        index = other.index;
        ptr = other.ptr;
        other.index = not_found;
        other.ptr = nullptr;
        return *this;
    }

    constexpr static_any(static_any const &other):
        index{other.index},
        ptr{other.ptr->clone()}
    {
    }

    constexpr static_any &operator=(static_any const &other)
    {
        release();
        index = other.index;
        ptr = other.ptr->clone();
    }
};
