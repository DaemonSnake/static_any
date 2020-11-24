# static_any: std::visit(std::any)

## Disclarer
- readme is not finished
- c++20 only library
- requires at least **gcc 10.1** or **clang 10.0.0**
- depends on my [unconstexpr](https://github.com/DaemonSnake/unconstexpr-cpp20) project
- **! `static_any<>` is not the same type as `static_any<>`, same goes for `result<X>` !**
- **!! don't prototype functions returning a `static_any<>` / seperating definition and declartion !!** think of any function with a `static_any<>` in his prototype as a `static inline` function


## first... example
### static_any<>
```c++
enum Type { INT, FLOAT, CHAR };

static_any<> default_value(Type t) {
   switch(t) {
      case INT: return int{};
      case FLOAT: return float{};
      case CHAR: return char{};
      default:
        return {};
   }
}

int main()
{
   auto r = default_value(INT);
   auto visitor = [](auto value) { /*...*/ };
   visit(r, visitor); //will call decltype(visitor)::operator()<int>
   r = "hello world";
   visit(r, visitor); //will call decltype(visitor)::operator()<const char *>
}
```
### result<X>
```c++
result<int> simple_test(err_codes i) {
  switch (i) {
    case ERROR_STRING:
      return raise("hello world");
    case ERROR_DOUBLE:
      return raise(3.14);
  }
  return ok_res;
}

result<int> use_result_simple(int) { /*...*/ }

result<int> use_simple(err_codes i) {
   return simple_test(i) | &use_result_simple;
   //equivalent to:
   // auto r = simple_test(i);
   // if (!r) return r;
   // return use_result_simple(r); //implicit cast to 'int', 'operator*' also available
}

int main() {
  auto r = use_simple(ERROR_DOUBLE);
  
  if (r.catch_except([](auto except) { /*received an exception*/ }) {
     //further error handling / cleanup
     return 1;
  }
  int v = r;
  //...

  //result also defines a pipeline operator
  auto r1 = result{52} |
    [](int i) { return use_simple(ERROR_DOUBLE); } |
    [](int j) -> const char * { return "hello world"; }; //will not be executed as the lhs is in error state
  static_assert(std::is_same_v<decltype(*r1), const char *>);
  bool found_error = r.catch_except([]<class T>(T value) {
     assert(std::is_same_v<T, double>);
  };
  assert(found_error);
  return 0;
}
```
## Wat!
The library proposed here provides a type `static_any<const int *Id = MAGIC>` that can be understood as:
- a visitable `std::any`
- a deduced/automatic `std::variant`

Basically instead of having to provide to `std::variant` the list of potential types desired,
`static_any<>` will deduce it by itself as the current translation unit is being compiled.

Each time its constructor `template<class T> static_any(T&&)` is instantiated with a new type,
said type will be added at the list of potential types to switch against when visiting the object.

This list of types is an [unconstexpr expresion](https://github.com/DaemonSnake/unconstexpr-cpp20):
a compile-time mutable expression.

It also provides a type `result<class Expect, const int *Id = MAGIC>` that utilize `static_any` to implement parts of (Zero-overhead deterministic exceptions)[http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r2.pdf].

the implicit return on exception isn't possible on the library level,
but it holds typed exceptions instead of relying on std::error_code.

