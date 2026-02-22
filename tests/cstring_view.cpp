//  SPDX-FileCopyrightText: 2026 Arnav Tuli
//  SPDX-License-Identifier: MIT

#include "../include/tuli/cstring_view.hpp"

#include <gtest/gtest.h>

#include "include/string_utils.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace tuli::cstring_view_literals;

TEST(CStringViewTest, DefaultConstructor) {
    constexpr tuli::cstring_view csv{};

    static_assert(csv.length() == 0);
    static_assert(csv.is_empty());
    static_assert(tests::strcmp(csv.c_str(), ""));
    static_assert(csv == ""sv);
}

TEST(CStringViewTest, FromCString) {
    constexpr auto s{"Hello, World!"};
    constexpr tuli::cstring_view csv{s};

    static_assert(csv.length() == tests::strlen(s));
    static_assert(!csv.is_empty());
    static_assert(tests::strcmp(csv.c_str(), s));
    static_assert(csv == std::string_view{s});
}

TEST(CStringViewTest, FromNullTerminatedCStringAndLen) {
    constexpr auto s{"Hello, World!"};
    constexpr auto len{tests::strlen(s)};
    constexpr tuli::cstring_view csv{tuli::null_terminated, s, len};

    static_assert(csv.length() == len);
    static_assert(!csv.is_empty());
    static_assert(tests::strcmp(csv.c_str(), s));
    static_assert(csv == std::string_view{s});
}

TEST(CStringViewTest, FromNullTerminatedStringView) {
    constexpr auto sv{"Hello, World!"sv};
    constexpr tuli::cstring_view csv{tuli::null_terminated, sv};

    static_assert(csv.length() == sv.length());
    static_assert(!csv.is_empty());
    static_assert(tests::strcmp(csv.c_str(), sv.data()));
    static_assert(csv == sv);
}

TEST(CStringViewTest, FromDefaultConstructedStringView) {
    constexpr tuli::cstring_view csv{tuli::null_terminated, std::string_view{}};

    static_assert(csv.length() == 0);
    static_assert(csv.is_empty());
    static_assert(tests::strcmp(csv.c_str(), ""));
    static_assert(csv == ""sv);
}

TEST(CStringViewTest, FromString) {
    const auto str{"Hello, World!"s};
    const tuli::cstring_view csv{str};

    EXPECT_EQ(csv.length(), str.length());
    EXPECT_FALSE(csv.is_empty());
    EXPECT_STREQ(csv.c_str(), str.c_str());
    EXPECT_EQ(csv, std::string_view{str});
}

TEST(CStringViewTest, FromDefaultConstructedString) {
    const auto str{std::string{}};
    const tuli::cstring_view csv{str};

    EXPECT_EQ(csv.length(), 0);
    EXPECT_TRUE(csv.is_empty());
    EXPECT_STREQ(csv.c_str(), "");
    EXPECT_EQ(csv, ""sv);
}

TEST(CStringViewTest, FromCStringViewLiteral) {
    constexpr auto csv{"Hello, World!"_csv};

    static_assert(csv.length() == tests::strlen("Hello, World!"));
    static_assert(!csv.is_empty());
    static_assert(tests::strcmp(csv.c_str(), "Hello, World!"));
    static_assert(csv == "Hello, World!"sv);
}

TEST(CStringViewTest, CopyConstructor) {
    constexpr auto csv1{"Hello, World!"_csv};
    constexpr tuli::cstring_view csv2{csv1};

    static_assert(csv1.length() == csv2.length());
    static_assert(csv1.is_empty() == csv2.is_empty());
    static_assert(tests::strcmp(csv1.c_str(), csv2.c_str()));
    static_assert(csv1 == std::string_view{csv2});
}

TEST(CStringViewTest, CopyAssignment) {
    constexpr auto csv1{"Hello, World!"_csv};
    tuli::cstring_view csv2{};

    EXPECT_NE(csv1.length(), csv2.length());
    EXPECT_NE(csv1.is_empty(), csv2.is_empty());
    EXPECT_STRNE(csv1.c_str(), csv2.c_str());
    EXPECT_NE(csv1, std::string_view{csv2});

    csv2 = csv1;

    EXPECT_EQ(csv1.length(), csv2.length());
    EXPECT_EQ(csv1.is_empty(), csv2.is_empty());
    EXPECT_STREQ(csv1.c_str(), csv2.c_str());
    EXPECT_EQ(csv1, std::string_view{csv2});
}

TEST(CStringViewTest, MoveConstructorShouldCopy) {
    constexpr auto csv1{"Hello, World!"_csv};
    constexpr tuli::cstring_view csv2{std::move(csv1)};

    static_assert(csv1.length() == csv2.length());
    static_assert(csv1.is_empty() == csv2.is_empty());
    static_assert(tests::strcmp(csv1.c_str(), csv2.c_str()));
    static_assert(csv1 == std::string_view{csv2});
}

TEST(CStringViewTest, MoveAssignmentShouldCopy) {
    constexpr auto csv1{"Hello, World!"_csv};
    tuli::cstring_view csv2{};

    EXPECT_NE(csv1.length(), csv2.length());
    EXPECT_NE(csv1.is_empty(), csv2.is_empty());
    EXPECT_STRNE(csv1.c_str(), csv2.c_str());
    EXPECT_NE(csv1, std::string_view{csv2});

    csv2 = std::move(csv1);

    EXPECT_EQ(csv1.length(), csv2.length());
    EXPECT_EQ(csv1.is_empty(), csv2.is_empty());
    EXPECT_STREQ(csv1.c_str(), csv2.c_str());
    EXPECT_EQ(csv1, std::string_view{csv2});
}

TEST(CStringViewTest, MoveAssignmentSelf) {
    constexpr auto s{"Hello, World!"};
    tuli::cstring_view csv{s};

    csv = std::move(csv);

    EXPECT_EQ(csv.length(), tests::strlen(s));
    EXPECT_FALSE(csv.is_empty());
    EXPECT_STREQ(csv.c_str(), s);
    EXPECT_EQ(csv, std::string_view{s});
}