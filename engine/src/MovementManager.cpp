#include <queue>
#include <stack>
#include <chrono>
#include <iostream>
#include "MovementManager.hpp"

namespace hive
{
    MovementManager::MovementManager() {}

    std::set<Position> MovementManager::getAvailableMoves(char type, const Position &position)
    {
        switch (type)
        {
        case 'G':
            return getGrasshopperMoves(position);
        case 'Q':
            return getQueenMoves(position);
        case 'B':
            return getBeetleMoves(position);
        case 'S':
            return getSpiderMoves(position);
        case 'A':
            return getAntMoves(position);
        default:
            return std::set<Position>();
        }
    }

    /*
    Ant:
    Może ruszyć się na dowolne pole do którego może fizycznie dojść.
    dfs po polach które są w zbiorze emptyTiles
    lista odwiedzonych pól to możliwe ruchy dla ant
    */
    std::set<Position> MovementManager::getAntMoves(Position position)
    {
        std::set<Position> visited;
        std::stack<Position> stack;

        for (auto &direction : directions)
        {
            Position neighbour = position + direction;
            if (emptyTiles.find(neighbour) != emptyTiles.end() && !isDirectionBlocked(position, direction, 1))
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

            for (const auto &direction : directions)
            {
                if (emptyTiles.find(current + direction) != emptyTiles.end() && visited.find(current + direction) == visited.end() && !isDirectionBlocked(current, direction, 1))
                {
                    stack.push(current + direction);
                }
            }
        }

        return visited;
    }

    /*
   BEETLE:
   Może się poruszać na sąsiednie pola na tym samym poziomie lub różnym o 1.
   */
    std::set<Position> MovementManager::getBeetleMoves(Position position) const
    {
        std::set<Position> moves;

        auto beetle_level = getLevel(position);

        for (const auto &direction : directions)
        {
            if (getLevel(position + direction) >= beetle_level)
            {
                int newBeetleLevel = getLevel(position + direction) + 1;
                if (!isDirectionBlocked(position, direction, newBeetleLevel))
                {
                    moves.insert(position + direction);
                }

                continue;
            }

            if (!isDirectionBlocked(position, direction, beetle_level))
                moves.insert(position + direction);
        }

        return moves;
    }


    /*
    SPIDER
    Ruch długości 3 bez zawracania oraz w każdym pośrednim polu musi się z kimś stykać.
    implementacja wymaga bfs który powiększa się najpierw w szerz a potem w głąb, żeby móc znaleść wszyskie ścieżki długości 3
    */
    std::set<Position> MovementManager::getSpiderMoves(Position position)
    {
        std::set<Position> visited;
        std::queue<std::pair<Position, int>> q;
        std::set<Position> spiderNeighbours = getNeighbours(position);
        std::set<Position> result;

        q.push({position, 0});
        visited.insert(position);

        while (!q.empty())
        {
            const auto &[currentPos, depth] = q.front();
            q.pop();

            if (depth == 3)
            {
                result.insert(currentPos);
                continue;
            }

            for (const auto &direction : directions )
            {
                Position neighbor = currentPos + direction;
                if (visited.find(neighbor) == visited.end() && emptyTiles.find(neighbor) != emptyTiles.end())
                {
                    int isSpiderNeighbour = (spiderNeighbours.find(neighbor) != spiderNeighbours.end()) ? 1 : 0;
                    if (calculateNeighbours(neighbor) - isSpiderNeighbour > 0)
                    {
                        if (isDirectionBlocked(currentPos, neighbor - currentPos, 1))
                        {
                            continue;
                        }
                        visited.insert(neighbor);
                        q.push({neighbor, depth + 1});
                    }
                }
            }
        }

        return result;
    }


    /*
    GRASSHOPPER:
    Przeskakuje przez figury na puste pole za nimi. W dowolnym kierunku.
    Musi przeskoczyć przez co najmniej jedną figurę.
    */

    std::set<Position> MovementManager::getGrasshopperMoves(Position position) const
    {
        std::set<Position> moves;

        for (auto &direction : directions)
        {
            Position current = position;

            if (isEmpty(current + direction))
            {
                continue;
            }

            while (!isEmpty(current))
            {
                current = current + direction;
            }

            moves.insert(current);
        }

        return moves;
    }

    /*
    Queen:
    Może się poruszać na sąsiednie pola
    */
    std::set<Position> MovementManager::getQueenMoves(Position position) const
    {
        std::set<Position> moves;
        for (auto &direction : directions)
        {
            if (isEmpty(position + direction) && !isDirectionBlocked(position, direction, 1))
            {
                moves.insert(position + direction);
            }
        }
        return moves;
    }
}