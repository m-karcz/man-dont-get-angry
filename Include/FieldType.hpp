#pragma once

#include <variant>
#include "PlayerId.hpp"

struct Home
{
    PlayerId owner;
};

struct Start
{
    PlayerId owner;
};

struct Regular
{};

struct None
{};

using FieldType = std::variant<Home,Start,Regular,None>;
