//  SPDX-FileCopyrightText: 2026 Arnav Tuli
//  SPDX-License-Identifier: MIT

#ifndef TESTS_STRING_UTILS_HPP
#define TESTS_STRING_UTILS_HPP

#include <cstddef>
#include <string>

namespace tests {

[[nodiscard]] constexpr bool strcmp(const char* s1, const char* s2) noexcept {
    if (!s1 || !s2) {
        return false;
    }

    while (*s1 && *s2) {
        if (*s1 != *s2) {
            return false;
        }
        ++s1;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        ++s2;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    return *s1 == *s2;
}

[[nodiscard]] constexpr std::size_t strlen(const char* s) noexcept {
    return std::char_traits<char>::length(s);
}

}  // namespace tests

#endif  // TESTS_STRING_UTILS_HPP