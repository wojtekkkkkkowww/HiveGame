#pragma once

#include <map>
#include <set>
#include <deque>
#include <list>
#include <vector>
#include "Tile.hpp"
#include "Position.hpp"

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
        void setBoardTiles(std::map<Position, std::deque<Tile>> &tiles);
        void addEmptyTilesAroundBoard();

        Position whiteQueen = invalidPosition;
        Position blackQueen = invalidPosition;

        std::list<Tile> getTiles() const;
        std::set<Position> getPlayerTiles(char color) const;
        std::vector<Position> getPositions() const;

        static std::set<Position> getNeighbours(Position position);

        std::map<Position, std::deque<Tile>> boardTiles;
        std::set<Position> emptyTiles;
        std::map<std::string, Position> positions;

        inline Tile getTileByNotation(std::string notation) const
        {
            if (positions.find(notation) == positions.end())
            {
                return Tile(notation);
            }

            Position position = positions.at(notation);

            return boardTiles.at(position).back();
        }

        inline int getLevel(Position position) const
        {
            if (isEmpty(position))
            {
                return 0;
            }

            return boardTiles.at(position).size();
        }

        inline void moveTile(Position position, Position newPosition)
        {
            auto tile = boardTiles.at(position).back();
            removeTile(position);
            addTile(newPosition, tile);
        }

        inline int calculateColoredNeighbours(Position position, char color) const
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

        inline int calculateNeighbours(const Position &position) const
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

        inline int getTileCount() const
        {
            return boardTiles.size();
        }

        inline bool isEmpty(Position position) const
        {
            return boardTiles.find(position) == boardTiles.end();
        }

        inline Tile getTile(Position position) const
        {
            const auto &tiles = boardTiles.at(position);
            return tiles.back();
        }

        inline void removeTile(Position position)
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

        inline void addTile(Position position, Tile tile)
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
}