#include "MovementManager.hpp"
#include <queue>
#include <stack>

namespace hive
{
MovementManager::MovementManager() = default;

// API: output parameter versions (no allocation)
void MovementManager::getAvailableMoves(char type, const Position &position,
                                        std::set<Position> &outMoves)
{
    outMoves.clear();
    switch (type)
    {
    case 'G':
        getGrasshopperMoves(position, outMoves);
        break;
    case 'Q':
        getQueenMoves(position, outMoves);
        break;
    case 'B':
        getBeetleMoves(position, outMoves);
        break;
    case 'S':
        getSpiderMoves(position, outMoves);
        break;
    case 'A':
        getAntMoves(position, outMoves);
        break;
    }
}

/*
Ant:
Can move to any tile it can physically reach.
DFS on tiles that are in the emptyTiles set.
The list of visited tiles is the possible moves for the ant.
*/
void MovementManager::getAntMoves(const Position &position, std::set<Position> &outMoves)
{
    outMoves.clear();

    std::stack<Position> stack;

    for (const auto &direction : directions)
    {
        Position neighbour = position + direction;
        if (emptyTiles.find(neighbour) != emptyTiles.end() &&
            !isDirectionBlocked(position, direction, 1))
        {
            stack.push(neighbour);
        }
    }

    while (!stack.empty())
    {
        Position current = stack.top();
        stack.pop();

        if (outMoves.find(current) != outMoves.end())
        {
            continue;
        }
        outMoves.insert(current);

        for (const auto &direction : directions)
        {
            if (emptyTiles.find(current + direction) != emptyTiles.end() &&
                outMoves.find(current + direction) == outMoves.end() &&
                !isDirectionBlocked(current, direction, 1))
            {
                stack.push(current + direction);
            }
        }
    }
}

/*
BEETLE:
Can move to adjacent tiles on the same level or on diferent level.
*/
void MovementManager::getBeetleMoves(const Position &position, std::set<Position> &outMoves) const
{
    outMoves.clear();

    auto beetle_level = getLevel(position);

    for (const auto &direction : directions)
    {
        if (getLevel(position + direction) >= beetle_level)
        {
            int newBeetleLevel = getLevel(position + direction) + 1;
            if (!isDirectionBlocked(position, direction, newBeetleLevel))
            {
                outMoves.insert(position + direction);
            }
            continue;
        }

        if (!isDirectionBlocked(position, direction, beetle_level))
        {
            outMoves.insert(position + direction);
        }
    }
}

/*
SPIDER
Moves exactly 3 spaces without backtracking and must touch another piece at each intermediate space.
Implementation requires BFS which expands breadth-first and then depth-first to find all paths of
length 3.
*/
void MovementManager::getSpiderMoves(const Position &position, std::set<Position> &outMoves)
{
    outMoves.clear();

    std::set<Position> visited;
    std::queue<std::pair<Position, int>> q;
    std::set<Position> spiderNeighbours = getNeighbours(position);

    q.push({position, 0});
    visited.insert(position);

    while (!q.empty())
    {
        const auto &[currentPos, depth] = q.front();
        q.pop();

        if (depth == 3)
        {
            outMoves.insert(currentPos);
            continue;
        }

        for (const auto &direction : directions)
        {
            Position neighbor = currentPos + direction;
            if (visited.find(neighbor) == visited.end() &&
                emptyTiles.find(neighbor) != emptyTiles.end())
            {
                int isSpiderNeighbour =
                    (spiderNeighbours.find(neighbor) != spiderNeighbours.end()) ? 1 : 0;
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
}

/*
GRASSHOPPER:
Jumps over pieces to an empty tile behind them. In any direction.
Must jump over at least one piece.
*/
void MovementManager::getGrasshopperMoves(const Position &position,
                                          std::set<Position> &outMoves) const
{
    outMoves.clear();

    for (const auto &direction : directions)
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

        outMoves.insert(current);
    }
}

/*
Queen:
Can move to adjacent tiles
*/
void MovementManager::getQueenMoves(const Position &position, std::set<Position> &outMoves) const
{
    outMoves.clear();

    for (const auto &direction : directions)
    {
        if (isEmpty(position + direction) && !isDirectionBlocked(position, direction, 1))
        {
            outMoves.insert(position + direction);
        }
    }
}
} // namespace hive