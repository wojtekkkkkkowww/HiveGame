#pragma once

#include "Position.hpp"
#include "StaticContainers.hpp"

namespace hive
{
class Board;

/**
 * @brief The ArticulationPointFinder class is responsible for finding articulation points in the
 * Hive game. This means points that, if removed, would split the hive into two or more separate
 * parts.
 */
class ArticulationPointFinder
{
  public:
    ArticulationPointFinder(Board &val);

    void findArticulationPoints();
    void prepareData();

  private:
    Board &val;

    static constexpr std::size_t maxHivePieces = 22;
    StaticVector<Position, maxHivePieces> tiles;
    StaticMap<Position, int, maxHivePieces> disc;
    StaticMap<Position, int, maxHivePieces> low;
    StaticSet<Position, maxHivePieces> ap;
    StaticSet<Position, maxHivePieces> visited;

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
} // namespace hive
