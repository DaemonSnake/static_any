#include "static_any.hpp"

#include <cxxabi.h>

#include <cstdio>
#include <iostream>
#include <string_view>
#include <typeinfo>

using StaticAny::static_any;
using StaticAny::visit;

constexpr static_any<> func(int i) {
  switch (i) {
    case 5:
      return 42;
    case 32:
      return 3.14;
    default:
      return "hello";
  }
}

template <class T>
void __attribute__((noinline)) require_fn(T&&) {
  char buf[1024];
  int status = 0;
  size_t length = 1024;
  abi::__cxa_demangle(typeid(T).name(), buf, &length, &status);
  buf[length] = '\0';
  std::printf("%s\n", buf);
}

int main() {
  auto item = func(5);
  auto lambda = [](auto i) { require_fn(i); };
  visit(item, lambda);
  item = false;
  visit(item, lambda);
  return 0;
}
