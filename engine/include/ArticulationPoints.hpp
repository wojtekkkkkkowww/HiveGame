#pragma once

#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>
#include <vector>
#include <map>
#include <stack>

namespace hive
{
    /**
     * @brief The ArticulationPointFinder class is responsible for finding articulation points in the Hive game.
     * This means points that, if removed, would split the hive into two or more separate parts.
     */
    class ArticulationPointFinder
    {
    public:
        ArticulationPointFinder(MoveValidator &val);

        void findArticulationPoints();
        void prepareData();

    private:
        MoveValidator &val;

        std::vector<Position> tiles;
        std::map<Position, int> disc;
        std::map<Position, int> low;
        std::set<Position> ap;
        std::set<Position> visited;

        int time;
        void dfsAP(Position u, Position parent);
        inline void updateLowAndCheckAP(Position u, Position v, Position parent);
        inline void updateLowForBackEdge(Position u, Position v);
        inline void checkRootAP(Position u, Position parent, int children);

    };

    inline void ArticulationPointFinder::updateLowAndCheckAP(Position u, Position v, Position parent)
    {
        low[u] = std::min(low[u], low[v]);

        if (parent != invalidPosition && low[v] >= disc[u])
        {
            ap.insert(u);
        }
    }

    inline void ArticulationPointFinder::updateLowForBackEdge(Position u, Position v)
    {
        low[u] = std::min(low[u], disc[v]);
    }

    inline void ArticulationPointFinder::checkRootAP(Position u, Position parent, int children)
    {
        if (parent == invalidPosition && children > 1)
        {
            ap.insert(u);
        }
    }
}
