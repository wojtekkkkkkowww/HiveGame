#include "dfs.hpp"
#include "BaseBoard.hpp"

namespace hive
{
    void DFS::explore(Position position)
    {
        visited.insert(position);

        for (const auto &neighbor : BaseBoard::getNeighbours(position))
        {
            if (tiles.find(neighbor) != tiles.end() && visited.find(neighbor) == visited.end())
            {   explore(neighbor);
            }
        }
    }

    std::set<Position> DFS::performDFS()
    {
        Position start = *tiles.begin();
        visited.clear();
        explore(start);
        return visited;
    }
}