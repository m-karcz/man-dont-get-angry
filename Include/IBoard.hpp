#pragma once

#include "PlayerId.hpp"
#include "FieldType.hpp"
#include "FieldId.hpp"
#include "Point.hpp"
#include <optional>
#include <vector>

struct IBoard
{
    virtual FieldType getFieldType(FieldId fieldId) const = 0;
    virtual Point getFieldLocation(FieldId fieldId) const = 0;
    virtual std::optional<FieldId> whatIsNextField(FieldId actualField, PlayerId playerId, int steps) const = 0;
    virtual FieldId getStartFieldOf(PlayerId playerId) const = 0;
    virtual std::vector<FieldId> getFieldList() const = 0;
    virtual ~IBoard() = default;
};
