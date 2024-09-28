#pragma once

#include "BaseBoard.hpp"
#include <vector>

namespace hive
{
    class MoveValidator : public virtual BaseBoard
    {
    public:
        MoveValidator &operator=(MoveValidator &&other) noexcept
        {
            if (this != &other)
            {
                BaseBoard::operator=(std::move(other));
            }
            return *this;
        }

        bool isMoveBlocked(Position position, Position newPosition);
        bool isOccupiedByOpponent(Position pos, std::string color);
        bool isQueenSurrounded(std::string color);

    private:
        bool isHiveConnectedAfterRemove(Position position);
        bool isDirectionBlocked(Position position, Position direction);
        bool isTouchingHiveAfterMove(Position position, Position newPosition);
    };
}