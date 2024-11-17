#pragma once

#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>
#include <vector>
#include <map>

namespace hive
{
    class ArticulationPointFinder
    {
    public:
        ArticulationPointFinder(const MoveValidator &val);

        std::set<Position> findArticulationPoints();

        void prepareData();

    private:
        const MoveValidator &val;

        std::vector<Position> tiles;
        std::map<Position, int> disc;
        std::map<Position, int> low;
        std::map<Position, bool> ap;
        std::map<Position, bool> visited;

        std::map<Position, int> myDict = {
            {Position{-2, 0}, 0},
            {Position{-1, 0}, 1},
            {Position{-2, 1}, 2},
            {Position{-3, 1}, 3},
            {Position{-1, 2}, 4},
            {Position{0, 0}, 5},
            {Position{0, 1}, 6},
            {Position{0, 2}, 7},
            {Position{-1, 3}, 8}};

        void print(Position v, std::string message = "")
        {
            if (myDict.find(v) != myDict.end())
            {
                std::cout << message << myDict[v] << std::endl;
            }
        }

        int time;

        std::vector<Position> getNeighbours(Position position) const;

        void dfsAP(Position u, Position parent);
    };
}
