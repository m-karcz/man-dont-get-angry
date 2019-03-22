#pragma once

#include "MessageInterest.hpp"
#include "TypeErasuredMessage.hpp"

enum class DispatchingStatus
{
    Successful,
    Unsuccessful
};

struct IDispatcher
{
    virtual void subscribe(MessageInterest interest) = 0;
    virtual DispatchingStatus dispatch(TypeErasuredMessage message) const = 0;
    virtual ~IDispatcher() = default;
};

