//  SPDX-FileCopyrightText: 2026 Arnav Tuli
//  SPDX-License-Identifier: MIT

#ifndef TULI_CSTRING_HPP
#define TULI_CSTRING_HPP

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace tuli {

inline constexpr struct owned_t {
} owned;

struct default_delete {
    void operator()(char* ptr) const {
        std::free(ptr);  // NOLINT(cppcoreguidelines-owning-memory,
                         // cppcoreguidelines-no-malloc)
    }
};

template <class Deleter = default_delete>
class cstring {  // NOLINT(cppcoreguidelines-special-member-functions)
  public:
    cstring(owned_t, char* s) noexcept : m_str{s} {
        if (m_str) {
            m_length = std::char_traits<char>::length(m_str.get());
        }
    }

    template <class D>
    cstring(owned_t, char* s, D&& d) noexcept : m_str{s, std::forward<D>(d)} {
        if (m_str) {
            m_length = std::char_traits<char>::length(m_str.get());
        }
    }

    cstring(cstring&& other) noexcept
        : m_str{std::move(other.m_str)},
          m_length{std::exchange(other.m_length, 0)} {}

    cstring& operator=(cstring&& other) noexcept {
        if (this != &other) {
            m_str = std::move(other.m_str);
            m_length = std::exchange(other.m_length, 0);
        }
        return *this;
    }

    [[nodiscard]] std::size_t length() const noexcept { return m_length; }

    [[nodiscard]] bool is_empty() const noexcept { return m_length == 0; }

    [[nodiscard]] bool is_null() const noexcept { return !m_str; }

    [[nodiscard]] const char* c_str() const noexcept { return m_str.get(); }

    [[nodiscard]] operator std::string_view() const {
        return std::string_view{m_str.get(), m_length};
    }

  private:
    std::unique_ptr<char, Deleter> m_str;
    std::size_t m_length{};
};

}  // namespace tuli

#endif  // TULI_CSTRING_HPP