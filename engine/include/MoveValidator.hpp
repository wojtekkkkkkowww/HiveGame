#pragma once

#include "BaseBoard.hpp"
#include <vector>

namespace hive
{
    /*
    * @brief The MoveValidator class is responsible for validating moves.
    */
    class MoveValidator : public BaseBoard
    {
    public:
        bool isMoveBlocked(Position position, Position newPosition) const;
        bool isOccupiedByOpponent(Position pos, char color) const;
        bool isQueenSurrounded(char color) const;
        bool isDirectionBlocked(Position position, Position direction, int level) const;

    private:
        bool isHiveConnectedAfterRemove(Position position) const;
        bool isTouchingHiveAfterMove(Position position, Position newPosition) const;
        bool constantContact(const hive::Position &neighborPosition1, const hive::Position &neighborPosition2, const hive::Position &newPosition) const;
        bool fredomToMove(const hive::Position &neighborPosition1, int level, const hive::Position &neighborPosition2) const;
    };
}