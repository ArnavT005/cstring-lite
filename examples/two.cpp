#if !defined(__APPLE__) || !defined(__MACH__)
#include <string.h>
#endif

#include <string_view>

#include "../include/tuli/cstring.hpp"
#include "../include/tuli/cstring_view.hpp"
#include "include/result.hpp"

namespace examples {

namespace detail {
char* some_c_api_two(const char* s) { return strdup(s); }
}  // namespace detail

[[nodiscard]] tuli::cstring<> some_c_api_wrapper_two(
    tuli::cstring_view csv) noexcept {
    return {tuli::owned, detail::some_c_api_two(csv.c_str())};
}

[[nodiscard]] result some_cpp_api(std::string_view sv) noexcept {
    const auto result_1{sv == "std::string_view"};
    const auto result_2{sv.substr(0, 3) == "std"};
    const auto result_3{sv.find("str") != std::string_view::npos};

    return result_1 && result_2 && result_3 ? result::SUCCESS : result::FAILURE;
}

using std::operator""sv;

[[nodiscard]] result two() noexcept {
    constexpr auto sv{"std::string_view"sv};
    const auto cstr{some_c_api_wrapper_two({tuli::null_terminated, sv})};

    return some_cpp_api(cstr);
}

}  // namespace examples