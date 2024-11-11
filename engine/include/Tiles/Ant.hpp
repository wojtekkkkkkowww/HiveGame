#pragma once
#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>
#include <stack>
#include <chrono>

namespace hive
{
    /*
    Ant:
    Może ruszyć się na dowolne pole do którego może fizycznie dojść.
    dfs po polach które są w zbiorze emptyTiles
    w taki sposób że do sąsiada można pójść tylko jeżeli dany kierunek jest dobry
    lista odwiedzonych pól to możliwe ruchy dla ant
    */
    namespace Ant
    {
        inline std::set<Position> getMoves(Position position, const MoveValidator &val, const std::set<Position> &emptyTiles)
        {
            std::set<Position> visited;
            std::stack<Position> stack;

            for (auto &direction : directions)
            {
                Position neighbour = position + direction;
                if (emptyTiles.find(neighbour) != emptyTiles.end() && !val.isDirectionBlocked(position, direction, 1))
                {
                    stack.push(neighbour);
                }
            }

            while (!stack.empty())
            {
                Position current = stack.top();
                stack.pop();

                if (visited.find(current) != visited.end())
                {
                    continue;
                }
                visited.insert(current);

                for (const Position &direction : directions)
                {
                    if (emptyTiles.find(current + direction) != emptyTiles.end() && visited.find(current + direction) == visited.end() && !val.isDirectionBlocked(current, direction, 1))
                    {
                        stack.push(current + direction);
                    }
                }
            }
           
            return visited;
        }
    }
}