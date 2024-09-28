#pragma once

#include "BaseBoard.hpp"
#include <set>

namespace hive
{
    class DFS
    {
    public:
        DFS(BaseBoard &board) : board(board) {}
        std::set<Position> performDFS(Position start);

    private:
        BaseBoard &board;
        std::set<Position> visited;
        void explore(Position position);
    };
}