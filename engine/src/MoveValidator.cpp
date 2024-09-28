#include "MoveValidator.hpp"
#include "dfs.hpp"

namespace hive
{
    bool MoveValidator::isMoveBlocked(Position position, Position newPosition)
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

        if (tile.type == "ANT")
        {
            return calculateNeighbours(newPosition, tile.color) >= 3;
        }
        else
        {
            return isDirectionBlocked(position, direction);
        }
    }

    bool MoveValidator::isOccupiedByOpponent(Position pos, std::string color)
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        return calculateNeighbours(pos, opponent) > 0;
    }

    bool MoveValidator::isQueenSurrounded(std::string color)
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        for (auto [key, tileList] : boardTiles)
        {
            auto tile = tileList.front();
            if (tile.color == color && tile.type == "QUEEN")
            {
                return calculateNeighbours(tile.position, color) + calculateNeighbours(tile.position, opponent) == 6;
            }
        }
        return false;
    }

    bool MoveValidator::isHiveConnectedAfterRemove(Position position)
    {
        Tile tile = getTile(position);
        removeTile(position);

        DFS dfs(*this);
        std::set<Position> visited = dfs.performDFS(boardTiles.begin()->first);

        std::set<Position> tilePositions;
        for (const auto &[pos, _] : boardTiles)
        {
            tilePositions.insert(pos);
        }

        addTile(position, tile);

        return visited == tilePositions;
    }

    bool MoveValidator::isDirectionBlocked(Position position, Position direction)
    {
        std::map<Position, std::vector<Position>> neighboringDirections = {
            {{0, 1}, {{1, 1}, {-1, 1}}},    // N -> NE, NW
            {{1, 1}, {{0, 1}, {1, -1}}},    // NE -> N, SE
            {{1, -1}, {{1, 1}, {0, -1}}},   // SE -> NE, S
            {{0, -1}, {{1, -1}, {-1, -1}}}, // S -> SE, SW
            {{-1, -1}, {{0, -1}, {-1, 1}}}, // SW -> S, NW
            {{-1, 1}, {{0, 1}, {-1, -1}}}   // NW -> N, SW
        };

        if (neighboringDirections.find(direction) == neighboringDirections.end())
        {
            std::cerr << "Error: Invalid direction provided." << std::endl;
            return true;
        }
        auto neighbors = neighboringDirections[direction];

        Position neighborPosition1 = {position.x + neighbors[0].x, position.y + neighbors[0].y};
        Position neighborPosition2 = {position.x + neighbors[1].x, position.y + neighbors[1].y};

        return !isEmpty(neighborPosition1) && !isEmpty(neighborPosition2);
    }

    bool MoveValidator::isTouchingHiveAfterMove(Position position, Position newPosition)
    {
        for (auto neighbour : getNeighbours(newPosition))
        {
            if (!isEmpty(neighbour) && neighbour != position)
            {
                return true;
            }
        }
        return false;
    }
}