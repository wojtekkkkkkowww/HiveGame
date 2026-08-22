#include "ArticulationPoints.hpp"
#include "Board.hpp"

namespace hive
{
ArticulationPointFinder::ArticulationPointFinder(Board &val) : val(val) {}

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

        if (!visited.contains(v))
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

    if (tiles.empty())
    {
        return;
    }

    dfsAP(tiles[0], invalidPosition);

    for (const auto &position : ap)
    {
        if (val.getLevel(position) > 1)
        {
            continue;
        }
        val.articulationPoints.insert(position);
    }
}

void ArticulationPointFinder::prepareData()
{
    tiles.clear();
    for (const auto &position : val.getPositions())
    {
        tiles.push_back(position);
    }
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
} // namespace hive