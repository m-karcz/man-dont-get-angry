#include <gtest/gtest.h>
#include <string>
#include "State.hpp"
#include "Board.hpp"

using namespace ::testing;

struct StateTestSuite : Test
{
    Board b;
};

TEST_F(StateTestSuite, shouldEvaluateEntryUntilFirstTrueCallback)
{
    State state;
    std::string result = "";
    state.onEntry([&](){result += "1"; return false;});
    state.onEntry([&](){result += "2"; return false;});
    state.onEntry([&](){result += "3"; return true;});
    state.onEntry([&](){result += "4"; return true;});

    ASSERT_EQ(result, "");

    state.enter();

    ASSERT_EQ(result, "123");
}

TEST_F(StateTestSuite, shouldEvaluateLeaveUntilFirstTrueCallback)
{
    State state;
    std::string result = "";
    state.onLeave([&](){result += "1"; return false;});
    state.onLeave([&](){result += "2"; return false;});
    state.onLeave([&](){result += "3"; return true;});
    state.onLeave([&](){result += "4"; return true;});

    ASSERT_EQ(result, "");

    state.leave();

    ASSERT_EQ(result, "123");
}

TEST_F(StateTestSuite, shoudReactOnlyOnMatchingEvent)
{
    State state;
    std::string result = "";
    state.onEvent<int>([&](const Event&){result += "int"; return true;});
    state.onEvent<bool>([&](const Event&){result += "bool"; return true;});
    state.onEvent<std::string>([&](const Event&){result += "string"; return true;});

    Event boolEvent(false);

    state.process(boolEvent);

    ASSERT_EQ(result, "bool");
}
