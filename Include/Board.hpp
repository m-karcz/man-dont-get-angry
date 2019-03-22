#pragma once

#include "IBoard.hpp"
#include <algorithm>
#include "overloaded.hpp"
#include <vector>
#include <array>
#include <map>


struct FieldInfo
{
    FieldType type;
    struct NextField
    {
        std::optional<FieldId> normal;
        std::optional<FieldId> special;
        bool (*isSpecialPredicate)(PlayerId);
    } next;
    Point location;
};

struct FieldIdComparator
{
    bool operator()(const FieldId& lhs, const FieldId& rhs) const
    {
        return lhs._ < rhs._;
    }
};

struct Board : IBoard
{
    FieldType getFieldType(FieldId fieldId) const override
    {
        return board.find(fieldId)->second.type;
    }
    Point getFieldLocation(FieldId fieldId) const override
    {
        return board.find(fieldId)->second.location;
    }
    std::optional<FieldId> whatIsNextField(FieldId actualField, PlayerId playerId, int steps) const override
    {
        if(steps < 1)
        {
            return std::nullopt;
        }
        auto it = board.find(actualField);
        if(it == board.end())
        {
            return std::nullopt;
        }
        const auto& info = it->second;
        bool isSpecial = info.next.isSpecialPredicate(playerId);
        const auto& next = isSpecial ? info.next.special : info.next.normal;
        if(not next.has_value())
        {
            return std::nullopt;
        }
        if(steps == 1)
        {
            return next;
        }
        return whatIsNextField(*next, playerId, steps - 1);
    }
    FieldId getStartFieldOf(PlayerId playerId) const
    {
        auto it = std::find_if(
            board.begin(),
            board.end(),
            [=](auto&& pair)
            {
                auto&& [id, info] = pair;
                return std::visit(overloaded{[=](const Home& home){return home.owner._ == playerId._;},
                                             [](auto&&){return false;}}, 
                                  info.type);
            });
        if(it != board.end())
        {
            return it->first;
        }
        throw std::logic_error{"asked for home of non-existent playerId"};
    }
    std::vector<FieldId> getFieldList() const override
    {
        std::vector<FieldId> list;
        list.reserve(board.size());
        std::transform(board.begin(),
                       board.end(),
                       std::back_inserter(list),
                       [](const auto& pair){return pair.first;});
        return list;
    }
protected:
    std::map<FieldId, FieldInfo, FieldIdComparator> board;
};



struct FourPlayersBoardBuilder : Board
{
    Board build()
    {
        std::array<std::vector<std::pair<FieldId, FieldInfo>>,4> quarters;
        for(int playerId : {0,1,2,3})
        {
            quarters[playerId] = makeQuarter(PlayerId{playerId});
        }
        for(int playerId : {0,1,2,3})
        {
            quarters[playerId].back().second.next.normal = quarters[(playerId + 1) % 4].front().first;
        }

        for(auto& quarter : quarters)
        {
            board.insert(quarter.begin(), quarter.end());
        }
        /*auto quarter = makeQuarter(PlayerId{0});

        board.insert(quarter.begin(), quarter.end());*/
        
        return *this;
    }
private:
    template<int dx, int dy> Point static moveField(Point before) { return {before.x + dx, before.y + dy}; }
    template<int Id> bool static isSpecial(PlayerId playerId) { return playerId._ == Id; }; 
    std::vector<std::pair<FieldId, FieldInfo>> makeQuarter(PlayerId playerId)
    {
        constexpr int HOME_SIZE = 4;
        constexpr int SIDE_SIZE = 4;
        std::vector<std::pair<FieldId, FieldInfo>> quarter;

        const auto primaryMove = [=](Point point){
            return player2direction[playerId._].first(point);
        };
        const auto secondaryMove = [=](Point point){
            return player2direction[playerId._].second(point);
        };

        const auto lastLocation = [&](){
            return quarter.back().second.location;
        };

        const auto mergeLast2Normal = [&](){
            quarter[quarter.size() - 2].second.next.normal = quarter.back().first;
        };

        const auto mergeLast2Special = [&](){
            quarter[quarter.size() - 2].second.next.special = quarter.back().first;
            quarter[quarter.size() - 2].second.next.isSpecialPredicate = playerIdPredicates[playerId._];
        };

        auto beforeHome = beforeHomes[playerId._];
        quarter.emplace_back(regularField(beforeHome));
        
        for(int i = 0; i < HOME_SIZE; i++)
        {
            quarter.emplace_back(nextFieldId(), FieldInfo{Home{}, {}, primaryMove(lastLocation())});
            mergeLast2Special();
        }
        
        quarter.emplace_back(nextFieldId(), FieldInfo{Start{}, {}, secondaryMove(beforeHome)});
        quarter.back().second.next.normal = quarter.front().first;

        for(int i = 0; i < SIDE_SIZE; i++)
        {
            quarter.emplace_back(nextFieldId(), FieldInfo{Regular{}, {}, primaryMove(lastLocation())});
            mergeLast2Normal();
        }
        for(int i = 0; i < SIDE_SIZE; i++)
        {
            quarter.emplace_back(nextFieldId(), FieldInfo{Regular{}, {}, secondaryMove(lastLocation())});
            mergeLast2Normal();
        }
        
        return quarter;
    }
    FieldId nextFieldId()
    {
        return FieldId{fieldId++};
    }
    std::pair<FieldId, FieldInfo> regularField(Point location)
    {
        return {nextFieldId(), {Regular{}, {}, location}};
    }
    int fieldId = 0;
    constexpr static std::array<std::pair<Point(*)(Point),Point(*)(Point)>,4> player2direction{{
        {moveField<1, 0>,moveField<0,-1>},
        {moveField<0, 1>,moveField<1, 0>},
        {moveField<-1,0>,moveField<0, 1>},
        {moveField<0,-1>,moveField<-1,0>}
    }};
    constexpr static std::array<Point, 4> beforeHomes{{
        {0,5},
        {5,0},
        {10,5},
        {5,10}
    }};
    constexpr static std::array<bool(*)(PlayerId), 4> playerIdPredicates{{
        isSpecial<0>,
        isSpecial<1>,
        isSpecial<2>,
        isSpecial<3>
    }};
};
