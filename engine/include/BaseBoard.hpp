#pragma once

#include <map>
#include <set>
#include <list>
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
        Tile getTile(Position position);
        void removeTile(Position position);
        void addTile(Position position, Tile &tile);
        bool isEmpty(Position position);
        int getLevel(Position position);
        std::set<Position> getNeighbours(Position position);
        std::set<Position> getPlayerTiles(std::string color);
        int calculateNeighbours(Position position, std::string color);

        std::map<Position, std::list<Tile>> boardTiles;
        std::set<Position> emptyTiles;

    private:
        void addEmptyTilesAroundBoard();
    };
}