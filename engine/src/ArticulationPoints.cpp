#include "ArticulationPoints.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <vector>

namespace hive
{
    ArticulationPointFinder::ArticulationPointFinder(MoveValidator &val)
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

                updateLowAndCheckAP(u, v, parent);
            }
            else if (v != parent)
            {
                updateLowForBackEdge(u, v);
            }
        }

        checkRootAP(u, parent, children);
    }

    void ArticulationPointFinder::findArticulationPoints()
    {
        prepareData();

        if (tiles.size() == 0)
        {
            return;
        }

        dfsAP(tiles.at(0), invalidPosition);

        for (auto &position : ap)
        {
            if (val.getLevel(position) > 1)
                continue;

            val.articulationPoints.insert(position);
        }
    }

    void ArticulationPointFinder::prepareData()
    {
        tiles = val.getPositions();
        val.articulationPoints.clear();
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