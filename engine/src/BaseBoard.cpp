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

    void BaseBoard::moveTile(Position position, Position newPosition)
    {
        auto tile = boardTiles.at(position).back();
        removeTile(position);
        addTile(newPosition, tile);
    }

    Tile BaseBoard::getTile(Position position) const
    {
        // if (isEmpty(position))
        // {
        //     throw std::invalid_argument("No tile at position " + std::to_string(position.x) + " " + std::to_string(position.y));
        // }

        const auto &tiles = boardTiles.at(position);
        return tiles.back();
    }

    /*
    potencjalnie źle jak coś się popsuje to to jest kandydat
    */
    Tile BaseBoard::getTileByNotation(std::string notation) const
    {
        if (positions.find(notation) == positions.end())
        {
            return Tile(notation);
        }

        return boardTiles.at(positions.at(notation)).back();
    }

    void BaseBoard::removeTile(Position position)
    {
        if (!isEmpty(position))
        {
            Tile tile = boardTiles[position].back();
            boardTiles[position].pop_back();
            if (boardTiles[position].empty())
            {
                boardTiles.erase(position);
            }

            positions.erase(tile.notation);
        }
    }

    void BaseBoard::addTile(Position position, Tile tile)
    {
        tile.setPosition(position);

        if (tile.type == 'Q' && tile.color == 'W')
        {
            whiteQueen = position;
        }
        if (tile.type == 'Q' && tile.color == 'B')
        {
            blackQueen = position;
        }

        if (isEmpty(position))
        {
            boardTiles[position] = std::deque<Tile>();
        }

        boardTiles[position].push_back(tile);
        positions[tile.notation] = position;

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

    int BaseBoard::calculateColoredNeighbours(Position position, char color) const
    {
        int count = 0;
        for (const auto &dir : directions)
        {
            if (!isEmpty(position + dir))
            {
                const auto &tile = getTile(position + dir);
                if (tile.color == color)
                {
                    count++;
                }
            }
        }
        return count;
    }

    int BaseBoard::calculateNeighbours(const Position& position) const
    {
        int count = 0;
        for (const auto &dir : directions)
        {
            if (!isEmpty(position + dir))
            {
                count++;
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
