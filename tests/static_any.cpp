#include "static_any.hpp"

#include <cxxabi.h>
#include <iostream>
#include <typeinfo>
#include <cassert>

using StaticAny::static_any;
using StaticAny::visit;

constexpr static_any<> func(char c) {
  switch (c) {
    case 'i':
      return 42;
    case 'f':
      return 3.14;
    default:
      return "unknown";
  }
}

template <class T>
std::string type_name(T&&) {
  char buf[1024]{};
  int status = 0;
  size_t length = 1024;
  abi::__cxa_demangle(typeid(T).name(), buf, &length, &status);
  return std::string{buf};
}

template<class Expect>
constexpr auto expect_type = [](auto i) -> void {
  assert(typeid(i) == typeid(Expect));
  std::cout << '"' << i << "\" of type => ";
};

int main() {
  using namespace std::literals;

  auto item = func('i');
  auto same_lambda = [](auto i) -> std::string { return type_name(i); };

  std::cout << "by stream: " << item << std::endl;

  visit(item, expect_type<int>);
  {
    std::optional<std::string> res = visit(item, same_lambda);
    assert(res.has_value());
    assert(*res == "int"sv);
    std::cout << *res << std::endl;
  }
  item = false;
  visit(item, expect_type<bool>);
  {
    std::optional<std::string> res = visit(item, same_lambda);
    assert(res.has_value());
    assert(*res == "bool"sv);
    std::cout << *res << std::endl;
  }
  return 0;
}
