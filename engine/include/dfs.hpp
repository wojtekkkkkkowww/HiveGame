#pragma once

#include "Board.hpp"
#include <set>

namespace hive
{
    class DFS
    {
    public:
        DFS(Board &board) : board(board) {}

        std::set<Position> performDFS(Position start);

    private:
        Board &board;
        std::set<Position> visited;

        void explore(Position position);
    };

    void DFS::explore(Position position)
    {
        visited.insert(position);

        for (const auto &neighbor : board.getNeighbours(position))
        {
            if(!board.isEmpty(neighbor) && visited.find(neighbor) == visited.end())
                explore(neighbor);
        }
    }

    std::set<Position> DFS::performDFS(Position start)
    {
        visited.clear();
        explore(start);
        return visited;
    }
}