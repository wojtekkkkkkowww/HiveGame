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

        if (tile.type != "BEETLE" && tile.type != "GRASSHOPPER")
        {
            return isDirectionBlocked(position, direction);
        }

        /*
        kiedy beetle sie rusza może mieć blokadę ale nie musi :|
        */
        if (tile.type == "BEETLE")
        {
            if (getLevel(position) == getLevel(newPosition) + 1)
            {
                return isDirectionBlocked(position, direction);
            }
        }

        return false;
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
            {N, {N, NE}},
            {NE, {N, SE}},
            {SE, {NE, S}},
            {S, {SE, SW}},
            {SW, {S, NW}},
            {NW, {N, SW}}};

        if (neighboringDirections.find(direction) == neighboringDirections.end())
        {
            std::cerr << "Error: Invalid direction provided. " << direction.x << " " << direction.y << std::endl;
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