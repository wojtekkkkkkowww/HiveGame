#include "dfs.hpp"

namespace hive
{
    void DFS::explore(Position position)
    {
        visited.insert(position);

        for (const auto &neighbor : board.getNeighbours(position))
        {
            if (!board.isEmpty(neighbor) && visited.find(neighbor) == visited.end())
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