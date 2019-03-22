#include <gtest/gtest.h>
#include "Dispatcher.hpp"
#include "Message.hpp"

using namespace ::testing;

struct DispatcherTestSuite : Test
{};

using Int2IntMessage = ::Message<int,int>;

TEST_F(DispatcherTestSuite, shouldProperlyDispatch)
{
    constexpr int VALUE_TO_BE_SET = 5;
    int valueToBeChanged = 0;

    Dispatcher sut;

    std::function<void(std::shared_ptr<Int2IntMessage>)> callback = [](std::shared_ptr<Int2IntMessage> message){message->commit(message->request);};

    sut.subscribe(interestByReactOn<Int2IntMessage>().with(callback));

    auto request = makeSharedMessage<Int2IntMessage>(VALUE_TO_BE_SET, [&](const int& value){valueToBeChanged = value;});

    ASSERT_NE(valueToBeChanged, VALUE_TO_BE_SET);

    auto dispatchingStatus = sut.dispatch(request);

    ASSERT_EQ(dispatchingStatus, DispatchingStatus::Successful);
    ASSERT_EQ(valueToBeChanged, VALUE_TO_BE_SET);
}

