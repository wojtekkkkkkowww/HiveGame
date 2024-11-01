#include "MoveValidator.hpp"
#include "dfs.hpp"

namespace hive
{
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
        return calculateNeighbours(pos, opponent) > 0;
    }

    bool MoveValidator::isQueenSurrounded(char color) const
    {
        char opponent = color == 'W' ? 'B' : 'W';
        Position position = (color == 'W') ? whiteQueen : blackQueen;
        if (position == invalidPosition)
        {
            return false;
        }
        return calculateNeighbours(position, color) + calculateNeighbours(position, opponent) == 6;
    }

    bool MoveValidator::isDirectionBlocked(Position position, Position direction, int level) const
    { 
        std::map<Position, std::vector<Position>> neighboringDirections = {
            {NW, {W, NE}},
            {NE, {NW, E}},
            {E, {NE, SE}},
            {SE, {E, SW}},
            {SW, {SE, W}},
            {W, {NW, SW}}};

        if (neighboringDirections.find(direction) == neighboringDirections.end())
        {
            return true;
        }

        auto neighbors = neighboringDirections[direction];

        Position neighborPosition1 = position + neighbors[0];
        Position neighborPosition2 = position + neighbors[1];

        if (fredomToMove(neighborPosition1, level, neighborPosition2))
        {
            std::cerr << "fredomToMove" << std::endl;
            return true;
        }

        if(boardTiles.size() == 1 || level != 1)
        {   
            return false;
        }

        Position newPosition = position + direction;
        if (constantContact(neighborPosition1, neighborPosition2, newPosition))
        {
            return true;
        }

        return false;
    }

    bool MoveValidator::constantContact(const hive::Position &neighborPosition1, const hive::Position &neighborPosition2, const hive::Position &newPosition) const
    {
        return isEmpty(neighborPosition1) && isEmpty(neighborPosition2) && getLevel(newPosition) == 0;
    }

    bool MoveValidator::fredomToMove(const hive::Position &neighborPosition1, int level, const hive::Position &neighborPosition2) const
    {
        return getLevel(neighborPosition1) >= level && getLevel(neighborPosition2) >= level;
    }

    /*
    sprawdzenie wymaga przejścia po wszystkich polach urzywając dfs
    corner case: Koło
    */
    bool MoveValidator::isHiveConnectedAfterRemove(Position position) const
    {
        if (getLevel(position) > 1)
        {
            return true;
        }

        std::set<Position> tilesPositions;
        for (auto [pos, _] : boardTiles)
        {
            if (pos != position)
                tilesPositions.insert(pos);
        }

        DFS dfs(tilesPositions);
        std::set<Position> visited = dfs.performDFS();

        return visited == tilesPositions;
    }

    bool MoveValidator::isTouchingHiveAfterMove(Position position, Position newPosition) const
    {
        for (auto neighbour : getNeighbours(newPosition))
        {
            if (!isEmpty(neighbour) && (neighbour != position || getLevel(position) > 1))
            {
                return true;
            }
        }
        return false;
    }
}