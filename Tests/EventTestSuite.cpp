#include <gtest/gtest.h>
#include "Event.hpp"

using namespace ::testing;

struct EventTestSuite : Test
{};

TEST_F(EventTestSuite, shouldPointToTypeFromCreation)
{
    int value = 5;
    Event event{value};

    ASSERT_TRUE(event.has<int>());
    ASSERT_FALSE(event.has<char>());
    ASSERT_FALSE(event.has<bool>());
}

TEST_F(EventTestSuite, shouldHaveSameValue)
{
    int value = 7;

    Event event{value};

    ASSERT_EQ(value, event.get<int>());
}

TEST_F(EventTestSuite, shouldThrowOnAccessToBadType)
{
    int value = 7777;

    Event event{value};

    ASSERT_THROW(event.get<bool>(), WrongTypeAccessException);
}
