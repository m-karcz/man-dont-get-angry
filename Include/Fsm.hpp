#pragma once

#include "Event.hpp"
#include "IState.hpp"

struct IFsm
{
    virtual void process Event(const Event& event) = 0;
    virtual ~IFsm() = default;
};

struct Fsm : IFsm
{
    void setInitialState(IState* initState)
    {
        actualState = initState;
        actualState->entry();
    }
    void processEvent(const Event& event) override
    {
        IState* oldState = actualState;
        actualState->processEvent(event);
        if(oldState != actualState)
        {
            oldState->leave();
            actualState->entry();
        }
    }
    void nextState(IState* state)
    {
        actualState = state;
    }
protected:
    IState* actualState = nullptr;
};
