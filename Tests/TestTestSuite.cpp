#include <gtest/gtest.h>

using namespace ::testing;

struct TestTestSuite : Test
{};

TEST_F(TestTestSuite, testLinking)
{
    ASSERT_TRUE(true);
}
