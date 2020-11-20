#include "result.hpp"

#include <cstdio>
#include <iostream>

using Result::raise;
using Result::result;

template <class... Args>
struct type_visitor : Args... {
  using Args::operator()...;
};

template <class... Args>
type_visitor(Args&&...) -> type_visitor<Args...>;

result<int> simple_test(int i) {
  switch (i) {
    case 0:
      return raise("hello");
    case 1:
      return raise(3.14);
  }
  return 42;
}

result<int> return_other_result(int j) {
  if (j == 2) return raise(42ul);
  return simple_test(j);
}

int main(int ac, const char** av) {
  int arg = [ac, &av]() {
    if (ac == 2)
      return std::atoi(av[1]);
    else
      return 2;
  }();

  auto x = return_other_result(arg);
  type_visitor on_error{
      [](const char* s) { std::printf("error '%s'\n", s); },
      [](double s) { std::printf("error '%f'\n", s); },
      [](auto s) { std::cout << "other error '" << s << "'" << std::endl; }};

  if (bool r = x.catch_except(on_error); r) {
    return 1;
  }
  std::printf("ok %d\n", *x);
  return 0;
}