#pragma once

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

}  // namespace StaticAny::Details