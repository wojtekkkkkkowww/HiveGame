#pragma once

#include "Position.hpp"
#include "Tile.hpp"
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace hive
{
/**
 * @brief The BaseBoard class is for managing tiles on the board.
 *
 */
class BaseBoard
{
  public:
    virtual ~BaseBoard() = default;

    void resetBoard();
    void setBoardTiles(
        const std::map<Position, std::deque<Tile>> &tiles); // to nie ma sensu bardzej move
    void addEmptyTilesAroundBoard();

    Position whiteQueen = invalidPosition;
    Position blackQueen = invalidPosition;

    std::vector<std::list<Tile>> getTiles() const;
    std::set<Position> getPlayerTiles(char color) const;
    std::vector<Position> getPositions() const;

    static std::set<Position> getNeighbours(Position position);

    std::map<Position, std::deque<Tile>> boardTiles;
    std::set<Position> emptyTiles;
    std::map<std::string, Position> positions;

    Tile getTileByNotation(const std::string &notation) const
    {
        if (positions.find(notation) == positions.end())
        {
            return {notation};
        }

        Position position = positions.at(notation);

        return boardTiles.at(position).back();
    }

    int getLevel(Position position) const
    {
        if (isEmpty(position))
        {
            return 0;
        }
        return static_cast<int>(boardTiles.at(position).size());
    }

    void moveTile(Position position, Position newPosition)
    {
        auto tile = boardTiles.at(position).back();
        removeTile(position);
        addTile(newPosition, tile);
    }

    int calculateColoredNeighbours(Position position, char color) const
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

    int calculateNeighbours(const Position &position) const
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

    int getTileCount() const { return static_cast<int>(boardTiles.size()); }

    bool isEmpty(Position position) const { return boardTiles.find(position) == boardTiles.end(); }

    Tile getTile(Position position) const
    {
        const auto &tiles = boardTiles.at(position);
        return tiles.back();
    }

    void removeTile(Position position)
    {
        if (!isEmpty(position))
        {
            const Tile tile = boardTiles[position].back();
            boardTiles[position].pop_back();
            if (boardTiles[position].empty())
            {
                boardTiles.erase(position);
            }
            positions.erase(tile.notation);
        }
    }

    void addTile(Position position, Tile tile)
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
};
} // namespace hive