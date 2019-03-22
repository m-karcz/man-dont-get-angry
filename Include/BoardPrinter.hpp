#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include "GetBoardMessage.hpp"

struct BoardPrinter
{
    BoardPrinter(Dispatcher& dispatcher)
    {
        dispatch = [&](){
            readStateImpl(dispatcher);
        };
    }
    void readStateFromBoard(const IBoard& iboard)
    {
        auto fieldIds = iboard.getFieldList();

        std::map<FieldId, Point> locations;

        for(const auto fieldId : fieldIds)
        {
            Point location = iboard.getFieldLocation(fieldId);

            locations[fieldId] = location;

            while(location.y >= board.size())
            {
                board.push_back(std::string{" "});
                if(board.size() > 1)
                {
                    board.back().resize(board.front().size(), ' ');
                }
            }
            if(location.x >= board.front().size())
            {
                for(auto& line : board)
                {
                    line.resize(location.x + 1, ' ');
                }
            }
            board[location.y][location.x] = std::visit(overloaded{[](Home){return 'H';},
                                                                  [](Regular){return 'O';},
                                                                  [](Start){return 'S';},
                                                                  [](auto&&){return 'x';}}, iboard.getFieldType(fieldId));
        }
    }
    void readStateImpl(Dispatcher& dispatcher)
    {
        auto msg = makeSharedMessage<GetBoardMessage>(GetBoardMessageRequest{}, [&](const IBoard& board){readStateFromBoard(board);});

        dispatcher.dispatch(msg);
    }
    void readState()
    {
        dispatch();
    }
    void print()
    {
        std::cout << "printed" << std::endl;
        std::copy(board.begin(), board.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    }
    std::vector<std::string> board;
    std::function<void()> dispatch;
};
