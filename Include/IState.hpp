#pragma once

#include "Event.hpp"

struct IState
{
    virtual void enter() = 0;
    virtual bool process(const Event& event) = 0;
    virtual void leave() = 0;
    virtual ~IState() = default;
};
