#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../../../src/myclass.h"
#include "../../../src/myclass2.h"

using namespace testing;

TEST(MyClass, appendString)
{
    MyClass mc;

    std::string str = mc.appendString("hello ", "test");

    EXPECT_EQ(str, "hello test");
    ASSERT_THAT(10, Eq(str.length()));
}

TEST(MyClass, appendNumber)
{
    MyClass mc;

    std::string str = mc.appendNumber("hello ", 10);

    EXPECT_EQ(str, "hello 10");
    ASSERT_THAT(8, Eq(str.length()));
}

TEST(MyClass2, appendNumber)
{
    MyClass2 mc;

    std::string str = mc.appendNumber("hello ", 10);

    EXPECT_EQ(str, "hello 10");
    ASSERT_THAT(8, Eq(str.length()));
}
