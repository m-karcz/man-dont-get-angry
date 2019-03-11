#pragma once

#include "IState.hpp"
#include "Event.hpp"
#include <vector>
#include <type_traits>

template<typename Func, typename ExpectedType, typename...Args>
constexpr bool returnsType = std::is_same_v<ExpectedType, std::invoke_result_t<Func, Args...>>;

template<typename Func, typename...Args>
constexpr bool returnsBool = returnsType<Func,bool,Args...>;

template<typename Func, typename...Args>
constexpr bool returnsVoid = returnsType<Func,void,Args...>;


struct State : IState
{
    using BooleanCallback = std::function<bool()>;
    using VoidCallback = std::function<void()>;
    template<typename EventType>
    using BooleanEventCallback = std::function<bool(const EventType&)>;
    template<typename EventType>
    using VoidEventCallback = std::function<void(const EventType&)>;
    using BooleanEventCallbackNontyped = std::function<bool(const Event&)>;
    /*template<typename EventType>
    void onEvent(BooleanEventCallback<EventType> callback)
    {
        eventCallbacks.push_back(
            [=](const Event& event)
            {
                return event.has<EventType>() && callback(event.get<EventType>());
            }
        );
    }
    template<typename EventType>
    void onEvent(VoidEventCallback<EventType> callback)
    {
        onEvent(toBooleanEvent(callback));
    }*/
    template<typename EventType, typename Callback>
    void onEvent(Callback callback)
    {
        static_assert(returnsBool<Callback,Event> or returnsVoid<Callback,Event>);
        if constexpr(returnsBool<Callback,Event>)
        {
            eventCallbacks.push_back(
                [=](const Event& event)
                {
                    return event.has<EventType>() && callback(event.get<EventType>());
                }
            );
        }
        else
        {
            onEvent<EventType>(toBooleanEvent(callback));
        }
    }
    template<typename Callback>
    void onEntry(Callback callback)
    {
        static_assert(returnsBool<Callback> or returnsVoid<Callback>);
        if constexpr(returnsBool<Callback>)
        {
            entryCallbacks.push_back(callback);
        }
        else if(returnsVoid<Callback>)
        {
            entryCallbacks.push_back(toBoolean(callback));
        }
    }
    template<typename Callback>
    void onLeave(Callback callback)
    {
        static_assert(returnsBool<Callback> or returnsVoid<Callback>);
        if constexpr(returnsBool<Callback>)
        {
            leaveCallbacks.push_back(callback);
        }
        else if(returnsVoid<Callback>)
        {
            leaveCallbacks.push_back(toBoolean(callback));
        }
    }
    void enter() override
    {
        callUntilTrue(entryCallbacks);
    }
    bool process(const Event& event)
    {
        for(auto& callback : eventCallbacks)
        {
            if(callback(event))
            {
                return true;
            }
        }
        return false;
    }
    void leave() override
    {
        callUntilTrue(leaveCallbacks);
    }
private:
    inline static BooleanCallback toBoolean(VoidCallback callback)
    {
        return [=](){callback(); return true;};
    }
    template<typename EventType>
    inline static BooleanEventCallback<EventType> toBooleanEvent(VoidEventCallback<EventType> callback)
    {
        return [=](const EventType& e){callback(e); return true;};
    }
    inline bool callUntilTrue(std::vector<BooleanCallback>& callbacks)
    {
        for(auto& callback : callbacks)
        {
            if(callback())
            {
                return true;
            }
        }
        return false;
    }
    std::vector<BooleanEventCallbackNontyped> eventCallbacks;
    std::vector<BooleanCallback> entryCallbacks;
    std::vector<BooleanCallback> leaveCallbacks;
};
