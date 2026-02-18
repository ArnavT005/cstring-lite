//  SPDX-FileCopyrightText: 2026 Arnav Tuli
//  SPDX-License-Identifier: MIT

#include "../include/tuli/cstring.hpp"

#include <gtest/gtest.h>

#include "include/string_utils.hpp"

TEST(CStringTest, FromCString) {
    constexpr auto s{"Hello, World!"};
    const tuli::cstring cstr{tuli::owned, strdup(s)};

    EXPECT_EQ(cstr.length(), tests::strlen(s));
    EXPECT_FALSE(cstr.is_empty());
    EXPECT_FALSE(cstr.is_null());
    EXPECT_STREQ(cstr.c_str(), s);
    EXPECT_EQ(cstr, std::string_view{s});
}

TEST(CStringTest, FromCStringWithValueTypeDeleter) {
    constexpr auto s{"Hello, World!"};

    int flag{};

    const struct deleter_t {
        void operator()(char* ptr) const {
            tuli::default_delete{}(ptr);
            ++(*flag_ptr);
        }
        int* flag_ptr;
    } deleter{&flag};

    // const deleter_t& constructor
    tuli::cstring<deleter_t>{tuli::owned, strdup(s), deleter};
    EXPECT_EQ(flag, 1);

    // deleter_t&& constructor
    tuli::cstring<deleter_t>{tuli::owned, strdup(s), deleter_t{&flag}};
    EXPECT_EQ(flag, 2);
}

TEST(CStringTest, FromCStringWithReferenceTypeDeleter) {
    constexpr auto s{"Hello, World!"};

    struct deleter_t {
        void operator()(char* ptr) {
            tuli::default_delete{}(ptr);
            ++flag;
        }
        int flag{};
    } deleter{};

    // deleter_t& constructor
    tuli::cstring<deleter_t&>{tuli::owned, strdup(s), deleter};
    EXPECT_EQ(deleter.flag, 1);
}

TEST(CStringTest, FromCStringWithConstReferenceTypeDeleter) {
    constexpr auto s{"Hello, World!"};

    int flag{};

    const struct deleter_t {
        void operator()(char* ptr) const {
            tuli::default_delete{}(ptr);
            ++(*flag_ptr);
        }
        int* flag_ptr;
    } deleter{&flag};

    // const deleter_t& constructor
    tuli::cstring<const deleter_t&>{tuli::owned, strdup(s), deleter};
    EXPECT_EQ(flag, 1);
}

TEST(CStringTest, FromEmptyCString) {
    constexpr auto s{""};
    const tuli::cstring cstr{tuli::owned, strdup(s)};

    EXPECT_EQ(cstr.length(), tests::strlen(s));
    EXPECT_TRUE(cstr.is_empty());
    EXPECT_FALSE(cstr.is_null());
    EXPECT_STREQ(cstr.c_str(), s);
    EXPECT_EQ(cstr, std::string_view{s});
}

TEST(CStringTest, FromNullptr) {
    const tuli::cstring cstr{tuli::owned, nullptr};

    EXPECT_EQ(cstr.length(), 0);
    EXPECT_TRUE(cstr.is_empty());
    EXPECT_TRUE(cstr.is_null());
    EXPECT_EQ(cstr.c_str(), nullptr);
}

TEST(CStringTest, MoveConstructor) {
    constexpr auto s{"Hello, World!"};
    tuli::cstring cstr1{tuli::owned, strdup(s)};
    const tuli::cstring cstr2{std::move(cstr1)};

    EXPECT_EQ(cstr1.length(), 0);
    EXPECT_TRUE(cstr1.is_empty());
    EXPECT_TRUE(cstr1.is_null());
    EXPECT_EQ(cstr1.c_str(), nullptr);

    EXPECT_EQ(cstr2.length(), tests::strlen(s));
    EXPECT_FALSE(cstr2.is_empty());
    EXPECT_FALSE(cstr2.is_null());
    EXPECT_STREQ(cstr2.c_str(), s);
    EXPECT_EQ(cstr2, std::string_view{s});
}

TEST(CStringTest, MoveAssignment) {
    constexpr auto s{"Hello, World!"};
    tuli::cstring cstr1{tuli::owned, strdup(s)};
    tuli::cstring cstr2{tuli::owned, nullptr};

    cstr2 = std::move(cstr1);

    EXPECT_EQ(cstr1.length(), 0);
    EXPECT_TRUE(cstr1.is_empty());
    EXPECT_TRUE(cstr1.is_null());
    EXPECT_EQ(cstr1.c_str(), nullptr);

    EXPECT_EQ(cstr2.length(), tests::strlen(s));
    EXPECT_FALSE(cstr2.is_empty());
    EXPECT_FALSE(cstr2.is_null());
    EXPECT_STREQ(cstr2.c_str(), s);
    EXPECT_EQ(cstr2, std::string_view{s});
}

TEST(CStringTest, MoveAssignmentSelf) {
    constexpr auto s{"Hello, World!"};
    tuli::cstring cstr{tuli::owned, strdup(s)};

    cstr = std::move(cstr);

    EXPECT_EQ(cstr.length(), tests::strlen(s));
    EXPECT_FALSE(cstr.is_empty());
    EXPECT_FALSE(cstr.is_null());
    EXPECT_STREQ(cstr.c_str(), s);
    EXPECT_EQ(cstr, std::string_view{s});
}