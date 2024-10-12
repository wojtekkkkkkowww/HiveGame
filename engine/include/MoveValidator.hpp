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

        bool isMoveBlocked(Position position, Position newPosition) const;
        bool isOccupiedByOpponent(Position pos, std::string color) const;
        bool isQueenSurrounded(std::string color) const;

    private:
        bool isHiveConnectedAfterRemove(Position position) const;
        bool isTouchingHiveAfterMove(Position position, Position newPosition) const;
    };
}