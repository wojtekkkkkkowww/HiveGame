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
        std::set<Position> articulationPoints;
        static std::map<Position, std::vector<Position>> neighboringDirections;

        inline bool isMoveBlocked(Position position, Position newPosition) const
        {
            return !isTouchingHiveAfterMove(position, newPosition);
        }

        inline bool isOccupiedByOpponent(Position pos, char color) const
        {
            char opponent = color == 'W' ? 'B' : 'W';
            return calculateColoredNeighbours(pos, opponent) > 0;
        }

        inline bool isQueenSurrounded(char color) const
        {
            Position position = (color == 'W') ? whiteQueen : blackQueen;
            if (position == invalidPosition)
            {
                return false;
            }
            return calculateNeighbours(position) == 6;
        }

        inline bool isDirectionBlocked(Position position, Position direction, int level) const
        {
            if (neighboringDirections.find(direction) == neighboringDirections.end())
            {
                return true;
            }

            const auto &neighbors = neighboringDirections[direction];

            if (fredomToMove(position + neighbors[0], level, position + neighbors[1]))
            {
                return true;
            }

            if (boardTiles.size() == 1 || level != 1)
            {
                return false;
            }

            if (constantContact(position + neighbors[0], position + neighbors[1], position + direction))
            {
                return true;
            }

            return false;
        }

        inline bool constantContact(hive::Position neighborPosition1, hive::Position neighborPosition2, hive::Position newPosition) const
        {
            return isEmpty(neighborPosition1) && isEmpty(neighborPosition2) && getLevel(newPosition) == 0;
        }

        inline bool fredomToMove(hive::Position neighborPosition1, int level, hive::Position neighborPosition2) const
        {
            return getLevel(neighborPosition1) >= level && getLevel(neighborPosition2) >= level;
        }

        inline bool isTouchingHiveAfterMove(Position position, Position newPosition) const
        {
            for (const auto &neighbour : getNeighbours(newPosition))
            {
                if (!isEmpty(neighbour) && (neighbour != position || getLevel(position) > 1))
                {
                    return true;
                }
            }
            return false;
        }
    };
}