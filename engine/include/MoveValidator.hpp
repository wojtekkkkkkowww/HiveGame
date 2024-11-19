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

        std::set<Position> articulationPoints;
    private:
        bool isHiveConnectedAfterRemove(Position position) const;
        bool isTouchingHiveAfterMove(Position position, Position newPosition) const;
        bool constantContact(hive::Position neighborPosition1, hive::Position neighborPosition2, hive::Position newPosition) const;
        bool fredomToMove(hive::Position neighborPosition1, int level, hive::Position neighborPosition2) const;

        static std::map<Position, std::vector<Position>> neighboringDirections;
    };
}