#include "MoveValidator.hpp"
#include "dfs.hpp"

namespace hive
{
    bool MoveValidator::isMoveBlocked(Position position, Position newPosition) const
    {
        try
        {
            if (!isHiveConnectedAfterRemove(position))
            {
                std::cerr << "Hive is not connected after remove" << std::endl;
                return true;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "1" << e.what() << '\n';
        }

        // do sprawdzenia
        try
        {
            if (!isTouchingHiveAfterMove(position, newPosition))
            {
                std::cerr << "Not touching hive after move" << std::endl;
                return true;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "2" << e.what() << '\n';
        }

        Tile tile = getTile(position);
        Position direction = {newPosition.x - position.x, newPosition.y - position.y};

        if (tile.type == "ANT")
        {
            return calculateNeighbours(newPosition, tile.color) >= 3;
        }

        try
        {
            if (tile.type != "BEETLE" && tile.type != "GRASSHOPPER")
            {
                return isDirectionBlocked(position, direction);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "3" << e.what() << '\n';
        }

        /*
        kiedy beetle sie rusza może mieć blokadę ale nie musi :|
        czyli to jest case kiedy trzeba sprawdzić czy jest blokada
        */
        try
        {
            if (tile.type == "BEETLE")
            {
                if (getLevel(position) == getLevel(newPosition) + 1)
                {
                    return isDirectionBlocked(position, direction);
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "4" << e.what() << '\n';
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
        for (auto [key, tiles] : boardTiles)
        {
            auto tile = tiles.back();
            if (tile.color == color && tile.type == "QUEEN")
            {
                return calculateNeighbours(tile.position, color) + calculateNeighbours(tile.position, opponent) == 6;
            }
        }
        return false;
    }

    /*
    nie wiarygone że to jest tak kosztowne żeby znaleść dziurę
    możesz mieć okrąg i go rozerwac żęby sprawdzić czy krańce są połaczone
    trzeba przejść cały
    */
    bool MoveValidator::isHiveConnectedAfterRemove(Position position) const
    {
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

    bool MoveValidator::isDirectionBlocked(Position position, Position direction) const
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