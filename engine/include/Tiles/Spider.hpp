#pragma once
#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>
#include <stack>
#include <chrono>
#include <queue>

namespace hive
{
    /*
    SPIDER
    Ruch długości 3 bez zawracania oraz w każdym pośrednim polu musi się z kimś stykać.
    implementacja wymaga bfs który powiększa się najpierw w szerz a potem w głąb, żeby móc znaleść wszyskie ścieżki długości 3
    */
    namespace Spider
    {

        inline std::set<Position> getMoves(Position position, const MoveValidator &val, const std::set<Position> &emptyTiles)
        {
            std::set<Position> visited;
            std::queue<std::pair<Position, int>> q;
            std::set<Position> spiderNeighbours = val.getNeighbours(position);
            std::set<Position> result;

            q.push({position, 0});
            visited.insert(position);

            while (!q.empty())
            {
                const auto& [currentPos, depth] = q.front();
                q.pop();

                if (depth == 3)
                {
                    result.insert(currentPos);
                    continue;
                }

                for (auto &neighbor : val.getNeighbours(currentPos))
                {
                    if (visited.find(neighbor) == visited.end() && emptyTiles.find(neighbor) != emptyTiles.end())
                    {
                        // Jeżeli pająk jest sąsiadem to go nie liczymy
                        int isSpiderNeighbour = (spiderNeighbours.find(neighbor) != spiderNeighbours.end()) ? 1 : 0;
                        if (val.calculateNeighbours(neighbor, 'B') + val.calculateNeighbours(neighbor, 'W') - isSpiderNeighbour > 0)
                        {
                            if (val.isDirectionBlocked(currentPos, neighbor - currentPos, 1))
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

    };

}