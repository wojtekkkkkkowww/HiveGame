#include "BaseBoard.hpp"

namespace hive
{

    void BaseBoard::resetBoard()
    {
        boardTiles.clear();
        emptyTiles.clear();
        positions.clear();
        emptyTiles.insert({0, 0});
        blackQueen = invalidPosition;
        whiteQueen = invalidPosition;
    }

    void BaseBoard::setBoardTiles(std::map<Position, std::deque<Tile>> &tiles)
    {
        for (const auto &[position, tile] : tiles)
        {
            for (const auto &t : tile)
            {
                addTile(position, t);
            }
        }

        addEmptyTilesAroundBoard();
    }

    std::vector<std::list<Tile>> BaseBoard::getTiles() const
    {
        size_t maxLevel = 0;
        for (const auto &[key, value] : boardTiles)
        {
            if (value.size() > maxLevel)
            {
                maxLevel = value.size();
            }
        }

        std::vector<std::list<Tile>> tiles(maxLevel);

        for (const auto &[key, value] : boardTiles)
        {
            int level = 0;
            for (const auto &tile : value)
            {
                tiles[level].push_back(tile);
                level++;
            }
        }

        return tiles;
    }

    std::set<Position> BaseBoard::getPlayerTiles(char color) const
    {
        std::set<Position> playerTiles;
        for (const auto &[key, tiles] : boardTiles)
        {
            const auto &tile = tiles.back();
            if (tile.color == color)
            {
                playerTiles.insert(tile.position);
            }
        }
        return playerTiles;
    }

    std::vector<Position> BaseBoard::getPositions() const
    {
        std::vector<Position> positions;
        for (const auto &[p, _] : boardTiles)
        {
            positions.push_back(p);
        }
        return positions;
    }

    void BaseBoard::addEmptyTilesAroundBoard()
    {
        emptyTiles.clear();
        for (const auto &[key, tiles] : boardTiles)
        {
            for (const auto &dir : directions)
            {
                Position neighbour = key + dir;
                if (isEmpty(neighbour))
                {
                    emptyTiles.insert(neighbour);
                }
            }
        }
    }

    std::set<Position> BaseBoard::getNeighbours(Position position)
    {
        std::set<Position> neighbours;

        for (auto &dir : directions)
        {
            neighbours.insert(position + dir);
        }

        return neighbours;
    }

}
