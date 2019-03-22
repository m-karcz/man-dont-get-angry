#pragma once

#include "Message.hpp"

struct IBoard;

struct GetBoardMessageRequest
{};

using GetBoardReaction = std::function<void(const IBoard&)>;

using GetBoardMessage = Message<GetBoardMessageRequest, IBoard>;
