#include "BaseBoard.hpp"

namespace hive
{
    void BaseBoard::resetBoard()
    {
        boardTiles.clear();
        emptyTiles.clear();
        emptyTiles.insert({0, 0});
    }

    Tile BaseBoard::getTile(Position position)
    {
        if (isEmpty(position))
        {
            throw std::invalid_argument("No tile at position");
        }
        auto tileList = boardTiles[position];
        auto tile = tileList.front();
        return tile;
    }

    void BaseBoard::removeTile(Position position)
    {
        if (!isEmpty(position))
        {
            boardTiles[position].pop_back();
            if (boardTiles[position].empty())
            {
                boardTiles.erase(position);
            }
        }
    }

    void BaseBoard::addTile(Position position, Tile &tile)
    {
        tile.setPosition(position);

        if (isEmpty(position))
        {
            boardTiles[position] = std::list<Tile>();
        }

        boardTiles[position].push_back(tile);
        addEmptyTilesAroundBoard();
    }

    bool BaseBoard::isEmpty(Position position)
    {
        return boardTiles.find(position) == boardTiles.end();
    }

    int BaseBoard::getLevel(Position position)
    {
        return boardTiles[position].size();
    }

    std::set<Position> BaseBoard::getNeighbours(Position position)
    {
        std::set<Position> directions = {N, S, NE, SW, NW, SE};
        std::set<Position> neighbours;

        for (auto &dir : directions)
        {
            neighbours.insert(position + dir);
        }

        return neighbours;
    }

    std::set<Position> BaseBoard::getPlayerTiles(std::string color)
    {
        std::set<Position> playerTiles;
        for (auto [key, tileList] : boardTiles)
        {
            auto tile = tileList.front();
            if (tile.color == color)
            {
                playerTiles.insert(tile.position);
            }
        }
        return playerTiles;
    }

    void BaseBoard::addEmptyTilesAroundBoard()
    {
        emptyTiles.clear();
        for (auto [key, tileList] : boardTiles)
        {
            auto tile = tileList.front();
            for (auto neighbourPosition : getNeighbours(tile.position))
            {
                if (isEmpty(neighbourPosition))
                {
                    emptyTiles.insert(neighbourPosition);
                }
            }
        }
    }

    int BaseBoard::calculateNeighbours(Position position, std::string color)
    {
        int count = 0;
        auto neighbours = getNeighbours(position);
        for (auto neighbour : neighbours)
        {
            if (!isEmpty(neighbour))
            {
                auto tile = getTile(neighbour);
                if (tile.color == color)
                {
                    count++;
                }
            }
        }
        return count;
    }
}