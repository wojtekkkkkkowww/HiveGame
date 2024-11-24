#include "ArticulationPoints.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <vector>

namespace hive
{
    ArticulationPointFinder::ArticulationPointFinder(const MoveValidator &val)
        : val(val) {}

    void ArticulationPointFinder::dfsAP(Position u, Position parent)
    {
        visited.insert(u);
        disc[u] = low[u] = ++time;
        int children = 0;

        for (const auto &d : directions)
        {
            Position v = u + d;
            if (val.isEmpty(v))
                continue;

            if (visited.find(v) == visited.end())
            {
                children++;
                dfsAP(v, u);

                // Post-recursion processing for `u` and `v`
                updateLowAndCheckAP(u, v, parent);
            }
            else if (v != parent)
            {
                // Back edge processing
                updateLowForBackEdge(u, v);
            }
        }

        // Root-specific articulation point check
        checkRootAP(u, parent, children);
    }

    std::set<Position> ArticulationPointFinder::findArticulationPoints()
    {
        prepareData();

        if (tiles.size() == 0)
        {
            return {};
        }

        dfsAP(tiles.at(0), invalidPosition);

        std::set<Position> articulationPoints;
        for (const auto &position : ap)
        {
            if (val.getLevel(position) > 1)
                continue;

            articulationPoints.insert(position);
        }

        return articulationPoints;
    }

    void ArticulationPointFinder::prepareData()
    {
        tiles = val.getPositions();

        disc.clear();
        low.clear();
        ap.clear();
        visited.clear();

        for (const Position &tile : tiles)
        {
            disc[tile] = 0;
            low[tile] = 0;
        }
        time = 0;
    }
}