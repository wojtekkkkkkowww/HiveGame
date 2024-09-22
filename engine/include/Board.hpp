#pragma once

#include <map>
#include <set>
#include <memory>
#include <list>
#include <stdexcept>

#include "Tile.hpp"
#include "MovementFunctions.hpp"

#define N {0, -1}
#define S {0, 1}
#define NE {1, -1}
#define SE {1, 0}
#define SW {-1, 1}
#define NW {-1, 0}

namespace hive
{
    class Board
    {
    public:
        Board();

        std::map<std::pair<int, int>, std::list<Tile>> boardTiles;
        std::set<std::pair<int, int>> emptyTiles;

        Tile getTile(std::pair<int, int> position);
        std::set<std::pair<int, int>> getNeighbours(std::pair<int, int> position);
        std::set<std::pair<int, int>> getAvailableMoves(Tile tile);

        bool isEmpty(std::pair<int, int> neighbourPosition);
        bool isOccupiedByOpponent(std::pair<int, int> pos, Color color);
        int calculateNeighbours(std::pair<int, int> position, Color color);
        void removeTile(std::pair<int, int> position);
        void addTile(std::pair<int, int> position, Tile tile);
        void resetBoard();
        void addEmptyTilesAroundBoard();
    };
}