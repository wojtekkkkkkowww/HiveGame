#include "MoveValidator.hpp"
#include "dfs.hpp"
#include <chrono>

namespace hive
{
    std::map<Position, std::vector<Position>> MoveValidator::neighboringDirections = {
        {NW, {W, NE}},
        {NE, {NW, E}},
        {E, {NE, SE}},
        {SE, {E, SW}},
        {SW, {SE, W}},
        {W, {NW, SW}}};

    bool MoveValidator::isMoveBlocked(Position position, Position newPosition) const
    {
        if (!isHiveConnectedAfterRemove(position))
        {
            return true;
        }

        if (!isTouchingHiveAfterMove(position, newPosition))
        {
            return true;
        }

        return false;
    }

    bool MoveValidator::isOccupiedByOpponent(Position pos, char color) const
    {
        char opponent = color == 'W' ? 'B' : 'W';
        return calculateColoredNeighbours(pos, opponent) > 0;
    }

    bool MoveValidator::isQueenSurrounded(char color) const
    {
        Position position = (color == 'W') ? whiteQueen : blackQueen;
        if (position == invalidPosition)
        {
            return false;
        }
        return calculateNeighbours(position) == 6;
    }

    bool MoveValidator::isDirectionBlocked(Position position, Position direction, int level) const
    {
        if (neighboringDirections.find(direction) == neighboringDirections.end())
        {
            return true;
        }

        const auto &neighbors = neighboringDirections[direction];

        if (fredomToMove(position + neighbors[0], level, position + neighbors[1]))
        {
            // std::cerr << "fredomToMove" << std::endl;
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

    bool MoveValidator::constantContact(hive::Position neighborPosition1, hive::Position neighborPosition2, hive::Position newPosition) const
    {
        return isEmpty(neighborPosition1) && isEmpty(neighborPosition2) && getLevel(newPosition) == 0;
    }

    bool MoveValidator::fredomToMove(hive::Position neighborPosition1, int level, hive::Position neighborPosition2) const
    {
        return getLevel(neighborPosition1) >= level && getLevel(neighborPosition2) >= level;
    }

    bool MoveValidator::isHiveConnectedAfterRemove(Position position) const
    {
        return articulationPoints.find(position) == articulationPoints.end();
    }

    bool MoveValidator::isTouchingHiveAfterMove(Position position, Position newPosition) const
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
}