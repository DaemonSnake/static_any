# static_any: std::visit(std::any)

## Disclarer
- readme is not finished
- c++20 header-only library
- requires at least **gcc 10.1** or **clang 10.0.0**
- depends on my [unconstexpr](https://github.com/DaemonSnake/unconstexpr-cpp20) project
- **! `static_any<>` is not the same type as `static_any<>`, same goes for `result<X>` !**
- **!! don't prototype functions returning a `static_any<>` / seperating definition and declartion !!** think of any function with a `static_any<>` in his prototype as a `static inline` function

## first... examples
### static_any<>
```c++
enum Type { INT, FLOAT, CHAR, ... };

static_any<> default_value(Type t) {
   switch(t) {
      case INT: return int{};
      case FLOAT: return float{};
      case CHAR: return char{};
      ...
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
   //if the visitor returns void, the return type of visit will be bool
   //otherwise std::optional<?>, where ? is the common return type of all possible calls to visitor's call operator
}
```
### result\<X>
```c++
result<int> some_fn(err_codes i) {
  switch (i) {
    case ERROR_STRING:
      return raise("hello world");
    case ERROR_DOUBLE:
      return raise(3.14);
  }
  return ok_res;
}

void print_int(int) { /*...*/ }
result<int> some_other_fn(int) { /*...*/ }

result<int> use_simple(err_codes i) {
   return some_fn(i) | &print_int | &some_other_fn;
   //equivalent to:
   // auto r = some_fn(i);
   // if (!r) return r;
   // print_int(*r);
   // return some_other_fn(*r);
}

int main() {
  auto r = use_simple(ERROR_DOUBLE);
  auto except_visit = [](auto except) -> void { ... };
  if (bool found_exception = catch_except(r, except_visit); found_exception) {
     //further error handling / cleanup
     return 1;
  }
  auto sizeof_visit = []<class T>(T const &) { return sizeof(T); }
  std::optional<size_t> size_of_exception = catch_except(r, sizeof_visit);
  if (size_of_exception) {
    ...
  }
  ...
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

It also provides a type `result<class Expect, const int *Id = MAGIC>` that utilize `static_any` to implement a staticly types and deterministic alternative to exceptions.
Inspired by rust's `Result` and (Zero-overhead deterministic exceptions)[http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r2.pdf].
