#pragma once

#include "BaseBoard.hpp"
#include <vector>

namespace hive
{
    class MoveValidator : public BaseBoard
    {
    public:

        bool isMoveBlocked(Position position, Position newPosition) const;
        bool isOccupiedByOpponent(Position pos, std::string color) const;
        bool isQueenSurrounded(std::string color) const;
        bool isDirectionBlocked(Position position, Position direction, int level) const;


    private:
        bool isHiveConnectedAfterRemove(Position position) const;
        bool isTouchingHiveAfterMove(Position position, Position newPosition) const;

    };
}