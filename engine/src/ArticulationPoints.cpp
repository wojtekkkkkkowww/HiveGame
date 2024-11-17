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

    std::vector<Position> ArticulationPointFinder::getNeighbours(Position position) const
    {
        std::vector<Position> neighbours;
        for (const Position &ng : val.getNeighbours(position))
        {
            if (!val.isEmpty(ng))
            {
                neighbours.push_back(ng);
            }
        }

        return neighbours;
    }

    void ArticulationPointFinder::dfsAP(Position u, Position parent)
    {
        visited[u] = true;
        disc[u] = low[u] = ++time;
        int children = 0;
        // print(u, "u: ");

        for (const Position &v : getNeighbours(u))
        {
            if (!visited[v])
            {
                // print(v, "v: ");

                children++;
                dfsAP(v, u);

                low[u] = std::min(low[u], low[v]);

                if (parent != invalidPosition && low[v] >= disc[u])
                {
                    // print(u, "AP: ");
                    ap[u] = true;
                }
            }
            else if (v != parent)
            {
                // print(v, "no parent: ");
                low[u] = std::min(low[u], disc[v]);
            }
        }

        if (parent == invalidPosition && children > 1)
        {
            // print(u, "ap: ");
            ap[u] = true;
        }
    }

    std::set<Position> ArticulationPointFinder::findArticulationPoints()
    {
        prepareData();

        if (tiles.size() == 0)
        {
            return {};
        }

        // for (const Position &tile : tiles)
        // {
        //     if (!visited[tile])
        //     {

        // for (const Position &pos : tiles)
        // {
        //
        // }

        // print(tiles[3]);
        dfsAP(tiles[3], invalidPosition);
        //     // }
        // }

        std::set<Position> articulationPoints;
        for (const auto &[position, isAP] : ap)
        {
            if (isAP)
            {
                // if there are more than one tile on the position, it is not an articulation point
                if (val.getLevel(position) > 1)
                    continue;

                articulationPoints.insert(position);
            }
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
            ap[tile] = false;
            visited[tile] = false;
        }
        time = 0;
    }
}