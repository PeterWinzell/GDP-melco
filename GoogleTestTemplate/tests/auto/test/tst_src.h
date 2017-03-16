#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(MyClass, myFunction)
{
    auto str = "hello test";
    EXPECT_EQ(str, "hello test");
    ASSERT_THAT(10, Eq(10));
}
