#include <cstdio>
#include <string>
#include <string_view>

#include "../include/tuli/cstring_view.hpp"
#include "include/result.hpp"

namespace examples {

namespace detail {
int some_c_api_one(const char* str) {
    return std::printf(  // NOLINT(cppcoreguidelines-pro-type-vararg)
        "%s says hello!\n", str);
}
}  // namespace detail

[[nodiscard]] int some_c_api_wrapper_one(tuli::cstring_view csv) noexcept {
    return detail::some_c_api_one(csv.c_str());
}

using std::operator""s;
using std::operator""sv;

[[nodiscard]] result one() noexcept try {
    constexpr auto s{"C-style string"};
    constexpr auto sv{"std::string_view (null-terminated)"sv};
    const auto str{"std::string"s};

    if (some_c_api_wrapper_one(s) < 0 ||
        some_c_api_wrapper_one({tuli::null_terminated, sv}) < 0 ||
        some_c_api_wrapper_one(str) < 0) {
        return result::FAILURE;
    }

    return result::SUCCESS;
} catch (...) {
    return result::FAILURE;
}

}  // namespace examples