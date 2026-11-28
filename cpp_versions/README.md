# C++ versions

Small, standalone programs for the features added in each C++ standard. One file teaches one feature. A file in `cpp17` may use C++11 and C++14, and it stays clear of C++20.

Each program links to the relevant page on [cppreference](https://en.cppreference.com), checks the behavior in `main`, and prints a line such as `OK: array`.

| Folder | Standard | Programs |
| --- | --- | ---: |
| [cpp11](cpp11) | C++11 | 38 |
| [cpp14](cpp14) | C++14 | 22 |
| [cpp17](cpp17) | C++17 | 43 |
| [cpp20](cpp20) | C++20 | 43 |
| [cpp23](cpp23) | C++23 | 30 |
| [cpp26](cpp26) | C++26 | 21 |
| [coroutine](coroutine) | C++20 coroutines, step by step | 7 |

There is no build system. Compile one file at a time. These commands were run with Apple clang 17.

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread -o /tmp/cpp17_optional cpp17/cpp17_optional.cpp
/tmp/cpp17_optional
```

To run every example in a folder, change the standard flag to match the directory (`c++11`, `c++14`, `c++17`, `c++20`, `c++23`, or `c++26`):

```bash
cd cpp20
mkdir -p /tmp/cpp20_build
for f in cpp20_*.cpp; do
  c++ -std=c++20 -Wall -Wextra -Wpedantic -Werror -pthread -o "/tmp/cpp20_build/${f%.cpp}" "$f" && "/tmp/cpp20_build/${f%.cpp}"
done
```

The `coroutine` directory uses `-std=c++20` as well. `cpp20/cpp20_coroutine.cpp` is the short checked example. The files in `coroutine` take the same mechanism apart.

## cpp11

`auto`, range-for, lambdas, move semantics, `unique_ptr` / `shared_ptr` / `weak_ptr`, `initializer_list`, `constexpr`, `enum class`, `override`, `= default` / `= delete`, delegating and inheriting constructors, member initializers, `static_assert`, `decltype`, variadic templates, alias templates, `noexcept`, raw string literals, `[[noreturn]]`, `nullptr`.

Library: `thread` and `lock_guard`, `condition_variable`, `async` / `future` / `promise`, `atomic`, `chrono`, `tuple`, `array`, `unordered_map`, `function` and `bind`, `<random>`, `<regex>`, `to_string` / `stoi`, `emplace`, type traits, `iota`, `current_exception`, `system_error`.

## cpp14

Generic lambdas, init-capture, return-type deduction, `decltype(auto)`, relaxed `constexpr`, variable templates, binary literals, digit separators, `[[deprecated]]`, aggregates with default member initializers.

Library: `make_unique`, `shared_timed_mutex`, `exchange`, `quoted`, `integer_sequence`, chrono and string literal suffixes, `std::get<Type>`, `_t` alias templates, heterogeneous lookup, four-iterator `equal` / `mismatch`, `cbegin`.

## cpp17

Structured bindings, `if constexpr`, if/switch with an initializer, fold expressions, class template argument deduction, inline variables, `template <auto>`, constexpr lambdas, `[*this]`, nested namespaces, guaranteed copy elision, aggregate bases, `__has_include`, `using` a pack of operators, `noexcept` as part of the function type, `[[nodiscard]]`, `[[maybe_unused]]`, `[[fallthrough]]`.

Library: `optional`, `variant`, `any`, `string_view`, `<filesystem>`, `sample`, `shared_mutex`, `scoped_lock`, `apply`, `invoke`, `std::byte`, `size` / `empty` / `data`, variable templates for traits, `from_chars` / `to_chars`, `try_emplace`, node handles, `not_fn`, `as_const`, `shared_ptr` to an array, `reduce`, polymorphic memory resources, `uncaught_exceptions`, chrono rounding, `clamp`, `gcd` / `lcm`.

## cpp20

Concepts, the three-way comparison operator, designated initializers, a range-for initializer, `consteval`, `constinit`, `[[likely]]` / `[[unlikely]]`, `char8_t`, `[[no_unique_address]]`, `explicit(bool)`, `using enum`, lambda template parameters, constexpr `std::vector` and `std::string`, `is_constant_evaluated`, a hand-written coroutine generator, bit-field improvements.

Library: ranges (`filter`, `transform`, `take`, `iota`, projections), `span`, `format`, `source_location`, `jthread`, `stop_source` / `stop_callback`, `barrier`, `latch`, semaphores, atomic wait/notify, `make_unique_for_overwrite`, `make_shared` for arrays, `midpoint`, `lerp`, `<numbers>`, `<bit>`, `starts_with` / `ends_with`, `contains` for maps and sets, `erase_if`, `ssize`, integer comparison functions, `bind_front`, `to_array`, the chrono calendar, `hardware_interference_size`.

`std::osyncstream` is part of C++20. This libc++ does not provide it, so `cpp20_barrier.cpp` guards its lines with a mutex.

## cpp23

`static_assert(false)` in a discarded branch, `[[assume]]`, `std::unreachable`, `if consteval`, explicit object parameters (deducing this), a multi-argument `operator[]`, `uz` / `z` suffixes, `auto(x)` decay-copy, a static call operator, `#elifdef` / `#elifndef`.

Library: `expected`, monadic operations on `optional`, `mdspan`, `flat_map`, `print` / `println`, `byteswap`, `to_underlying`, `bind_back`, `forward_like`, `invoke_r`, `ranges::zip`, `ranges::to`, `chunk_by` and `repeat`, `fold_left`, `ranges::contains` and `find_last`, `string::contains`, formatting ranges, `resize_and_overwrite`, constexpr `unique_ptr`, `is_scoped_enum`.

This libc++ does not provide `flat_set`, `std::generator`, `std::move_only_function`, `<spanstream>`, `<stacktrace>`, `<stdfloat>`, `fold_left_first`, or the views `enumerate`, `cartesian_product`, `chunk`, `stride`, `slide`, `pairwise`, and `join_with`. The hand-written generator is `cpp20_coroutine.cpp`.

## cpp26

Pack indexing, the `_` placeholder, `= delete("reason")`, a `static_assert` message that is a `string_view`, an attribute on one structured binding, and a constexpr cast from `void*`.

Library: saturating arithmetic, `span::at` and a span built from an `initializer_list`, `string` concatenated with `string_view`, `bitset` from a `string_view`, string streams from a `string_view`, `native_handle` on file streams, `not_fn` with a template argument, comparisons of `reference_wrapper`, `std::complex` as a tuple, `{:p}` / `{:P}` pointer formatting, `runtime_format`, `println()` with no format string, `std::dims`, `bool` conversion of `from_chars_result`, and range algorithms over pointers to an incomplete type.

This Apple clang does not accept contracts. It also has no `<debugging>`, `<linalg>`, `std::function_ref`, `std::copyable_function`, `std::inplace_vector`, `std::submdspan`, or `views::concat`. `#embed` is diagnosed as a C23 extension under `-Wpedantic`.

## coroutine

These seven files are the C++20 coroutine mechanism, compiled with `-std=c++20`. Read them in this order:

1. `handle.cpp` publishes a `coroutine_handle` through an awaiter.
2. `return.cpp` converts the return object to a type-erased handle.
3. `promise.cpp` retrieves the promise from an awaiter.
4. `yield.cpp` stores each `co_yield` in the promise.
5. `co_return.cpp` runs a finite coroutine and stops on `done()`.
6. `coroutine_handle.cpp` is a move-only generator used as a range.
7. `generator.cpp` yields a Fibonacci sequence.

`cpp20/cpp20_coroutine.cpp` is the same idea in one checked file.
