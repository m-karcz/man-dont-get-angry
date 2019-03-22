#pragma once

#include "IDispatcher.hpp"

struct Dispatcher : IDispatcher
{
    void subscribe(MessageInterest interest) override
    {
        interests.insert(std::make_pair(interest.typeInfo, interest));
    }
    DispatchingStatus dispatch(TypeErasuredMessage message) const override
    {
        auto it = interests.find(message.typeInfo);
        if(it != interests.end())
        {
            it->second.react(std::move(message));
            return DispatchingStatus::Successful;
        }
        return DispatchingStatus::Unsuccessful;
    }
private:
    std::map<TypeInfo, MessageInterest> interests;
};

