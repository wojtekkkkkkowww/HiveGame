#pragma once

#include <map>
#include <set>
#include <deque>
#include "Tile.hpp"
#include "Position.hpp"

namespace hive
{
    class BaseBoard
    {
    public:
        virtual ~BaseBoard() = default;
        BaseBoard &operator=(BaseBoard &&other) noexcept
        {
            if (this != &other)
            {
                boardTiles = std::move(other.boardTiles);
                emptyTiles = std::move(other.emptyTiles);
            }
            return *this;
        }

        void resetBoard();
        void removeTile(Position position);
        void addTile(Position position, Tile &tile);
        Tile getTile(Position position) const;
        bool isEmpty(Position position) const;
        int getLevel(Position position) const;
        std::set<Position> getPlayerTiles(std::string color) const;
        int calculateNeighbours(Position position, std::string color) const;

        static std::set<Position> getNeighbours(Position position);

        std::map<Position, std::deque<Tile>> boardTiles;
        std::set<Position> emptyTiles;

    private:
        void addEmptyTilesAroundBoard();
    };
}