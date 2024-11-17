#pragma once

#include "BaseBoard.hpp"
#include <set>

namespace hive
{
    class DFS
    {
    public:
        DFS(std::set<Position> &tiles) : tiles(tiles) {}
        std::set<Position> performDFS(Position start);

    private:
        std::set<Position> &tiles;
        std::set<Position> visited;
        void explore(Position position);
    };
}