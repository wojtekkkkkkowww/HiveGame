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

        int time;
        std::vector<Position> getNeighbours(Position position) const;
        void dfsAP(Position u, Position parent);
    };
}
