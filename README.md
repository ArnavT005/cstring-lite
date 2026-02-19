# cstring-lite

[![tests-multi-platform](https://github.com/ArnavT005/cstring-lite/actions/workflows/tests-multi-platform.yml/badge.svg)](https://github.com/ArnavT005/cstring-lite/actions/workflows/tests-multi-platform.yml)
[![examples-multi-platform](https://github.com/ArnavT005/cstring-lite/actions/workflows/examples-multi-platform.yml/badge.svg)](https://github.com/ArnavT005/cstring-lite/actions/workflows/examples-multi-platform.yml)
[![cpp-linter](https://github.com/ArnavT005/cstring-lite/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/ArnavT005/cstring-lite/actions/workflows/cpp-linter.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A lightweight C++ header-only library for safe and efficient interoperation with C APIs that either accept or return C-style strings.

## Motivation

C++ codebases often find themselves using C APIs for certain functionalities. These APIs can sometimes accept a null-terminated `const char*` string (*non-owning*) as an input argument, or return a heap-allocated `char*` string (*owning*) as their output, or both. The current C++ standard defines `std::string_view` and `std::string` classes to represent *non-owning* and *owning* string types respectively. However, these do not always safely or efficiently interoperate with C APIs:

- A `std::string_view` object cannot always be safely passed to a C API accepting a null-terminated `const char*` string since the former does not guarantee null-termination.
> [!NOTE]
> `std::string` type does provide the null-termination guarantee, but it's an *owning* type and can lead to unnecessary allocations if not used carefully.
- On the other hand, a `std::string` object cannot be constructed from a heap-allocated `char*` string returned by a C API without performing a **deep copy**. This double allocation can sometimes be undesirable, especially when the intention is only to perform string operations on the returned value.
> [!NOTE]
> `std::string_view` type can be constructed from the returned `char*` string to facilitate string operations, but it's a *non-owning* type and can lead to a memory leak if not used carefully.

Therefore, there is a need for string types that can bridge the gap between standard C++ strings and raw C-style strings for better interoperation.

## Bridge Types

To address the challenges outlined above, this repository introduces two types: `tuli::cstring_view` and `tuli::cstring`. These types are designed to safely and efficiently bridge the gap between C++ and C string semantics, making interoperation seamless and less error-prone.


### tuli::cstring_view

A lightweight, *non-owning* view over a null-terminated C-style string. Unlike `std::string_view`, `tuli::cstring_view` guarantees null-termination, making it safe to pass directly to C APIs expecting a null-terminated `const char*` string. It can be constructed from a `const char*`, `std::string`, or a null-terminated `std::string_view` object, and provides utility methods to get the length, check for emptiness, and access the underlying C-style string.

**Key features:**
- Guarantees null-termination for safe C API interop
- Non-owning, zero-overhead abstraction
- Implicit conversion to `std::string_view` for C++ compatibility
- User-defined literal operator (e.g. `"example"_csv`) for convenient construction

### template \<class Deleter\> tuli::cstring

An *owning*, RAII-enabled wrapper for heap-allocated C-style strings. This type takes ownership of the `char*` string returned by a C API, manages its lifetime, and ensures proper deallocation upon scope exit. It avoids unnecessary deep copies and memory leaks, while still allowing string operations via implicit conversion to `std::string_view`.

**Key features:**
- Owns and manages the lifetime of a heap-allocated `char*` string
- Move-only semantics to prevent accidental copies
- Provides `length()`, `is_empty()`, `is_null()`, and `c_str()` utility methods
- Implicit conversion to `std::string_view` for C++ compatibility
- Ensures memory is freed automatically, preventing leaks

> [!NOTE]
> If unspecified, `tuli::default_delete` is used as the default `Deleter`. Internally, this calls `std::free()` on the allocated `char*` string upon scope exit, making it suitable for memory allocated using standard C allocators such as `malloc`, `calloc`, or `realloc`.

> [!TIP]
> Some C APIs also provide custom deallocator methods to free previously allocated `char*` strings. To enable proper cleanup in such cases, a custom deleter type can be passed as the template parameter when using `tuli::cstring`. The requirements for such a deleter are the same as those imposed by the `std::unique_ptr` type.

## Installation

Just copy the files present in `include/` directory to your repository's include directory and you are ready to go!  
**No separate compiling/linking required.**

## Running Tests

This project uses [GoogleTest](https://github.com/google/googletest) to write and run tests. All test cases are located in the `tests/` directory. To run the test cases, follow the steps given below:

> [!IMPORTANT]
> **Prerequisites:**
> - CMake version **3.24** or higher
> - A C++ compiler with **C++17** support

1) Clone the repository

```
git clone https://github.com/ArnavT005/cstring-lite.git
cd cstring-lite
```

2) Build project with `TULI_ENV_BUILD_TESTS` environment variable set to `TRUE`

```
cmake -B build -DTULI_ENV_BUILD_TESTS=TRUE -S .
cmake --build build
```

3) Run the tests

You can run the tests using CTest:

```
cd build
ctest --test-dir tests/
```

Or run the test binary directly:

```
./build/tests/cstring-lite-tests
```

> [!NOTE]
> [GoogleTest](https://github.com/google/googletest) is automatically downloaded by CMake during the first build, so no manual setup is required.

## Usage Examples

Sample examples are present in the `examples/` directory. The following snippets illustrate the usage scenarios:

- Using `tuli::cstring_view` for making safe and efficient C API wrappers

```cpp
#include <cstdlib>
#include <string>
#include <string_view>

#include <tuli/cstring_view.hpp>

extern int some_c_api(const char* str); // returns -ve value on failure

[[nodiscard]] int some_c_api_wrapper(tuli::cstring_view csv) noexcept {
  return some_c_api(csv.c_str());
}

using std::operator""s;
using std::operator""sv;

int main() {
  const auto s{"C-style string"};
  const auto sv{"std::string_view (null-terminated)"sv};
  const auto str{"std::string"s};

  if (some_c_api_wrapper(s) < 0 ||
      some_c_api_wrapper({tuli::null_terminated, sv}) < 0 ||
      some_c_api_wrapper(str) < 0 ) {
        return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
```

- Using `tuli::cstring` to safely and efficiently wrap heap-allocated `char*` string and use it as a string type

```cpp
#include <string_view>

#include <tuli/cstring>
#include <tuli/cstring_view>

extern char* some_c_api(const char* str); // returns malloced duplicated 'str'

[[nodiscard]] tuli::cstring<> some_c_api_wrapper(tuli::cstring_view csv) noexcept {
  return {tuli::owned, some_c_api(csv.c_str())};
}

[[nodiscard]] bool some_cpp_api(std::string_view sv) noexcept {
    const auto result_1{sv == "std::string_view"};
    const auto result_2{sv.substr(0, 3) == "std"};
    const auto result_3{sv.find("str") != std::string_view::npos};

    return (result_1 && result_2 && result_3);
}

using std::operator""sv;

int main() {
    const auto sv{"std::string_view"sv};
    const auto cstr{some_c_api_wrapper({tuli::null_terminated, sv})};

    return some_cpp_api(cstr) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

## Running Examples

To build and run examples, follow the steps given below: (for prerequisites and cloning instructions, see [Running Tests](#running-tests) section)

1) Build project with `TULI_ENV_BUILD_EXAMPLES` environment variable set to `TRUE`

```
cmake -B build -DTULI_ENV_BUILD_EXAMPLES=TRUE -S .
cmake --build build
```

2) Run the examples

```
./build/examples/cstring-lite-examples
```

Expected output:

```
C-style string says hello!
std::string_view (null-terminated) says hello!
std::string says hello!
Examples ran successfully.
```

## Related Works and Discussion

### cstring_view
A number of proposals have been made to introduce a null-terminated `cstring_view` type in the C++ standard library. These include the following: 
- [P1402R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1402r0.pdf): std::cstring_view - a C compatible std::string_view adapter (*rejected*)
- [P3655R3](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3655r3.html): std::cstring_view (proposed for inclusion in *C++29*)

Implementations for the same can also be found on GitHub [here](https://github.com/breyerml/cstring_view) and [here](https://github.com/bemanproject/cstring_view). Though these solve the null-termination issue in similar ways, the definition of `cstring_view` class in above works is quite extensive given that most of the functionalities can be simply achieved via an implicit conversion to `std::string_view`. This seems rather unnecessary as such a type should ideally not substitute the use of `std::string_view` in C++ code, but only used when necessary (at the C/C++ boundary).

### cstring
`tuli::cstring<Deleter>` is essentially a one-to-one wrapper over `std::unique_ptr<char, Deleter>`, so a `std::unique_ptr` can, in theory, replace the usage of `tuli::cstring`. However, `tuli::cstring` offers some semantic advantages over `std::unique_ptr`:

- `tuli::cstring` makes the programmer's intention clear to the reader and is more expressive. `std::unique_ptr` implementation can possibly lead to errors if not used carefully.
- `tuli::cstring` provides implicit conversion to `std::string_view`, which makes performing string operations on the returned `char*` string smoother and less error-prone.
