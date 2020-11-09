#include <type_traits>
#include <tuple>
#include <memory>
#include <optional>

#include "static_any.hpp"

using namespace StaticAny;

template<class> struct printer;

//EXAMPLE
constexpr static_any<> func(int i)
{
    switch(i)
    {
        case 5:
            return 42;
        case 32:
            return 3.14;
        default:
            return "hello";
    }
}

#include <typeinfo>
#include <cstdio>
#include <cxxabi.h>

template<class T> void __attribute__((noinline)) require_fn(T&&) {
    char buf[1024];
    int status = 0;
    size_t length = 1024;
    abi::__cxa_demangle(typeid(T).name(), buf, &length, &status);
    buf[length] = '\0';
    std::printf("%s\n", buf);
}

#include <string_view>

int main()
{
    auto item = func(5);
    auto lambda = [](auto i) noexcept {
        require_fn(i);
    };
    visit(item, lambda);
    item = false;
    visit(item, lambda);
}
