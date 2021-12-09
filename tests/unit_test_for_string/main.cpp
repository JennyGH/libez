#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <gtest/gtest.h>
#include <base/string.hpp>

TEST(ez, string_test)
{
    EXPECT_STREQ(ez::base::string("aaa/bbb/ccc").replace("aaa", "").c_str(), "/bbb/ccc");
    EXPECT_STREQ(ez::base::string("aaa/bbb/ccc").replace("aaa", "bbb").c_str(), "bbb/bbb/ccc");
    EXPECT_STREQ(ez::base::string("aaa/bbb/ccc").replace("/", "-").c_str(), "aaa-bbb-ccc");
    EXPECT_STREQ(ez::base::string("哈/哈哈/哈").replace("/", "-").c_str(), "哈-哈哈-哈");
}

int main(int argc, char** argv)
{
#if MSVC
    system("chcp 65001");
#endif // MSVC
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}