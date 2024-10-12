#include "dfs.hpp"
#include "BaseBoard.hpp"
#include <stack>

/*
funkcja wykorzystywana do sprawdzenia spójności roju
*/
namespace hive
{
    void DFS::explore(Position start)
    {
        std::stack<Position> stack;
        stack.push(start);

        while (!stack.empty())
        {
            Position current = stack.top();
            stack.pop();

            if (visited.find(current) != visited.end())
            {
                continue;
            }

            visited.insert(current);

            for (const auto &neighbor : BaseBoard::getNeighbours(current))
            {
                if (tiles.find(neighbor) != tiles.end() && visited.find(neighbor) == visited.end())
                {
                    stack.push(neighbor);
                }
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