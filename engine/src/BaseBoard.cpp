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

    std::list<Tile> BaseBoard::getTiles() const
    {
        std::list<Tile> tiles;
        for (const auto &[key, value] : boardTiles)
        {
            for (const auto &tile : value)
            {
                tiles.push_back(tile);
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

    /*
    possible to make emptyTiles updates during adding and removing tiles
    */
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

    // void BaseBoard::removeEmptyTilesAroundPosition(Position position)
    // {   if(isEmpty(position))
    //     {
    //        if(emptyTiles.find(position) != emptyTiles.end())
    //        {
    //            emptyTiles.erase(position);
    //        }
    //     }

    //     for (const auto &dir : directions)
    //     {
    //         Position neighbour = position + dir;
    //         if (isEmpty(neighbour))
    //         {
    //             if (emptyTiles.find(neighbour) != emptyTiles.end())
    //                 emptyTiles.erase(neighbour);
    //         }
    //     }
    // }

    // void BaseBoard::addEmptyTilesAroundPosition(Position position)
    // {
    //     for (const auto &dir : directions)
    //     {
    //         Position neighbour = position + dir;
    //         if (isEmpty(neighbour))
    //         {
    //             if (emptyTiles.find(neighbour) == emptyTiles.end())
    //                 emptyTiles.insert(neighbour);
    //         }
    //     }
    // }

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
