#pragma once

#include "Any.hpp"

struct TypeErasuredMessage
{
    template<typename Message>
    TypeErasuredMessage(std::shared_ptr<Message> message) :
        message{message},
        typeInfo{TypeInfo::get<Message>()}
    {}
    Any message;
    TypeInfo typeInfo;
};
