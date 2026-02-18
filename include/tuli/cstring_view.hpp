//  SPDX-FileCopyrightText: 2026 Arnav Tuli
//  SPDX-License-Identifier: MIT

#ifndef TULI_CSTRING_VIEW_HPP
#define TULI_CSTRING_VIEW_HPP

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>

namespace tuli {

inline constexpr struct null_terminated_t {
} null_terminated;

class cstring_view {
  public:
    constexpr cstring_view() noexcept : m_sv{""} {}

    constexpr cstring_view(const char* s) : m_sv{s} {
        assert(m_sv.data() != nullptr &&
               "tuli::cstring_view cannot be constructed from nullptr");
    }

    constexpr cstring_view(null_terminated_t, std::string_view sv) noexcept
        : m_sv{sv.data() ? sv : ""} {}

#if defined(__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
    constexpr cstring_view(const std::string& str) noexcept : m_sv{str} {}
#else
    cstring_view(const std::string& str) noexcept : m_sv(str) {}
#endif

    [[nodiscard]] constexpr std::size_t length() const noexcept {
        return m_sv.length();
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return m_sv.empty();
    }

    [[nodiscard]] constexpr const char* c_str() const noexcept {
        return m_sv.data();
    }

    [[nodiscard]] constexpr operator std::string_view() const noexcept {
        return m_sv;
    }

  private:
    std::string_view m_sv;
};

inline namespace literals {
inline namespace cstring_view_literals {

[[nodiscard]] constexpr cstring_view operator""_csv(const char* s,
                                                    std::size_t len) noexcept {
    return cstring_view{null_terminated, std::operator""sv(s, len)};
}

}  // namespace cstring_view_literals
}  // namespace literals

}  // namespace tuli

#endif  // TULI_CSTRING_VIEW_HPP