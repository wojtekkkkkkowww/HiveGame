#include "BaseBoard.hpp"

namespace hive
{
    void BaseBoard::resetBoard()
    {
        boardTiles.clear();
        emptyTiles.clear();
        tiles.clear();
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

    void BaseBoard::setBoardTiles(std::map<Position, std::deque<std::shared_ptr<Tile>>> &tiles)
    {
        for (const auto &[position, tile] : tiles)
        {
            for (const auto &t : tile)
            {
                addTile(position, *t);
            }
        }

        addEmptyTilesAroundBoard();
    }

    void BaseBoard::moveTile(Position position, Position newPosition)
    {
        auto tile = boardTiles.at(position).back();
        removeTile(position);
        addTile(newPosition, tile);
    }

    Tile BaseBoard::getTile(Position position) const
    {
        if (isEmpty(position))
        {
            throw std::invalid_argument("No tile at position " + std::to_string(position.x) + " " + std::to_string(position.y));
        }

        const auto &tiles = boardTiles.at(position);
        return *tiles.back();
    }

    Tile BaseBoard::getTileByNotation(std::string notation) const
    {
        if (tiles.find(notation) == tiles.end())
        {
            return Tile(notation);
        }

        return *tiles.at(notation);
    }

    std::set<Position> BaseBoard::getEmptyTiles() const
    {
        return emptyTiles;
    }

    void BaseBoard::removeTile(Position position)
    {
        if (!isEmpty(position))
        {
            Tile tile = *boardTiles[position].back();
            boardTiles[position].pop_back();
            if (boardTiles[position].empty())
            {
                boardTiles.erase(position);
            }

            tiles.erase(tile.notation);
        }
    }

    void BaseBoard::addTile(Position position, Tile tile)
    {
        auto tilePtr = std::make_shared<Tile>(tile);

        addTile(position, tilePtr);
    }

    void BaseBoard::addTile(Position position, std::shared_ptr<Tile> tile)
    {
        tile->setPosition(position);

        if (tile->type == 'Q' && tile->color == 'W')
        {
            whiteQueen = position;
        }
        if (tile->type == 'Q' && tile->color == 'B')
        {
            blackQueen = position;
        }

        if (isEmpty(position))
        {
            boardTiles[position] = std::deque<std::shared_ptr<Tile>>();
        }

        boardTiles[position].push_back(tile);
        tiles[tile->notation] = tile;

        addEmptyTilesAroundBoard();
    }

    bool BaseBoard::isEmpty(Position position) const
    {
        return boardTiles.find(position) == boardTiles.end();
    }

    int BaseBoard::getLevel(Position position) const
    {
        if (isEmpty(position))
        {
            return 0;
        }

        return boardTiles.at(position).size();
    }

    std::list<Tile> BaseBoard::getTiles() const
    {
        std::list<Tile> tiles;
        for (const auto &[key, value] : boardTiles)
        {
            for (const auto &tile : value)
            {
                Tile newTile = *tile;
                tiles.push_back(newTile);
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
            if (tile->color == color)
            {
                playerTiles.insert(tile->position);
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
            const auto &tile = tiles.back();
            // tutaj zrobić ten trik z dir
            for (const auto &neighbourPosition : getNeighbours(tile->position))
            {
                if (isEmpty(neighbourPosition))
                {
                    emptyTiles.insert(neighbourPosition);
                }
            }
        }
    }

    int BaseBoard::calculateNeighbours(Position position, char color) const
    {
        int count = 0;
        auto neighbours = getNeighbours(position);
        for (const auto &neighbour : neighbours)
        {
            if (!isEmpty(neighbour))
            {
                const auto &tile = getTile(neighbour);
                if (tile.color == color)
                {
                    count++;
                }
            }
        }
        return count;
    }

    int BaseBoard::getTileCount() const
    {
        return boardTiles.size();
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