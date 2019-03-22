#pragma once

#include "Any.hpp"
#include "TypeErasuredMessage.hpp"
#include <functional>

using TypeErasuredMessageFunctor = std::function<void(TypeErasuredMessage)>;

struct MessageInterest
{
    const TypeInfo typeInfo;
    TypeErasuredMessageFunctor react;
};

template<typename Type>
struct TypedMessageInterestBuilder
{
    template<typename Callback>
    MessageInterest with(Callback callback) &&
    {
        TypeErasuredMessageFunctor typeErasuredCallback{[=](TypeErasuredMessage message){
            callback(message.message.get<std::shared_ptr<Type>>());
        }};
        return MessageInterest{TypeInfo::get<Type>(), std::move(typeErasuredCallback)};
    }
};

template<typename Type>
TypedMessageInterestBuilder<Type> interestByReactOn()
{
    return {};
}

/*struct MessageIntersetBuilder
{
    
};*/
