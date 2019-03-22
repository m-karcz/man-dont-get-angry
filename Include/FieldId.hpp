#pragma once

#include "StrongTypedef.hpp"

struct FieldId : StrongTypedef<int>
{};

bool operator<(const FieldId& lhs, const FieldId& rhs)
{
    return lhs._ < rhs._;
}
