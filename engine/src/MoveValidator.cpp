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

        Tile tile = getTile(position);
        Position direction = {newPosition.x - position.x, newPosition.y - position.y};

        if (tile.type == "QUEEN")
        {
            return isDirectionBlocked(position, direction);
        }

        if (tile.type == "BEETLE")
        {

            if (getLevel(position) == getLevel(newPosition) + 1)
            {
                return isDirectionBlocked(position, direction);
            }
        }

        return false;
    }

    bool MoveValidator::isOccupiedByOpponent(Position pos, std::string color) const
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        return calculateNeighbours(pos, opponent) > 0;
    }

    bool MoveValidator::isQueenSurrounded(std::string color) const
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        Position position = (color == "WHITE") ? whiteQueen : blackQueen;
        if (position == invalidPosition)
        {
            return false;
        }
        return calculateNeighbours(position, color) + calculateNeighbours(position, opponent) == 6;
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