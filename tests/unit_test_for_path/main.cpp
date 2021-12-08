#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <gtest/gtest.h>
#include <base/path.h>

TEST(ez, unix_path)
{
    EXPECT_STREQ(ez::base::path("/").to_string(ez::base::path::unix_style).c_str(), "/");
    EXPECT_STREQ(ez::base::path(".").to_string(ez::base::path::unix_style).c_str(), ".");
    EXPECT_STREQ(ez::base::path("..").to_string(ez::base::path::unix_style).c_str(), "..");
    EXPECT_STREQ(ez::base::path("./").to_string(ez::base::path::unix_style).c_str(), ".");
    EXPECT_STREQ(ez::base::path("../").to_string(ez::base::path::unix_style).c_str(), "..");
    EXPECT_STREQ(ez::base::path("a/b/c").to_string(ez::base::path::unix_style).c_str(), "a/b/c");
    EXPECT_STREQ(ez::base::path("/a/b/c").to_string(ez::base::path::unix_style).c_str(), "/a/b/c");
    EXPECT_STREQ(ez::base::path("aaa").to_string(ez::base::path::unix_style).c_str(), "aaa");
}

TEST(ez, windows_path)
{
    EXPECT_STREQ(ez::base::path("C:").to_string(ez::base::path::windows_style).c_str(), "C:");
    EXPECT_STREQ(ez::base::path(".").to_string(ez::base::path::windows_style).c_str(), ".");
    EXPECT_STREQ(ez::base::path(".\\").to_string(ez::base::path::windows_style).c_str(), ".");
    EXPECT_STREQ(ez::base::path("..").to_string(ez::base::path::windows_style).c_str(), "..");
    EXPECT_STREQ(ez::base::path("..\\").to_string(ez::base::path::windows_style).c_str(), "..");
    EXPECT_STREQ(ez::base::path("a\\b\\c").to_string(ez::base::path::windows_style).c_str(), "a\\b\\c");
    EXPECT_STREQ(ez::base::path(".\\a\\b\\c").to_string(ez::base::path::windows_style).c_str(), ".\\a\\b\\c");
    EXPECT_STREQ(ez::base::path("C:\\a\\b\\c").to_string(ez::base::path::windows_style).c_str(), "C:\\a\\b\\c");
    EXPECT_STREQ(ez::base::path("aaa").to_string(ez::base::path::windows_style).c_str(), "aaa");
}

int main(int argc, char** argv)
{
#if MSVC
    system("chcp 65001");
#endif // MSVC
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}