#include "result.hpp"

#include <cassert>
#include <iostream>
#include <typeindex>
#include <cxxabi.h>

using Result::raise;
using Result::result;

enum err_codes : int {
  ERROR_STRING = 0,
  ERROR_DOUBLE = 1,
  ERROR_SIZE_T = 2,
};

constexpr int ok_res = 42;

result<int> simple_test(err_codes i) {
  switch (i) {
    case ERROR_STRING:
      return raise("hello world");
    case ERROR_DOUBLE:
      return raise(3.14);
  }
  return ok_res;
}

result<int> return_other_result(err_codes j) {
  if (j == ERROR_SIZE_T) return raise(42ul);
  return simple_test(j);
}

template <class Expect, class Result>
void expect_error(Result&& res) {
  auto on_error = [](auto i) { return std::type_index{typeid(i)}; };
  auto opt_type = res.catch_except(on_error);
  assert(opt_type);
  assert(*opt_type == std::type_index{typeid(Expect)});

  char buf[1024]{};
  {
    int status = 0;
    size_t length = 1024;
    abi::__cxa_demangle(opt_type->name(), buf, &length, &status);
  }
  std::cerr << "[err] " << buf << std::endl;
}

int main(int ac, const char** av) {
  auto arg = ac != 2 ? ERROR_DOUBLE : static_cast<err_codes>(std::atoi(av[1]));
  auto res = return_other_result(arg);

  switch (arg) {
    case ERROR_STRING:
      expect_error<const char*>(res);
      break;
    case ERROR_DOUBLE:
      expect_error<double>(res);
      break;
    case ERROR_SIZE_T:
      expect_error<size_t>(res);
      break;
    default:
      assert(*res == ok_res);
      std::cout << "[ok] " << *res << std::endl;
  }
  return 0;
}