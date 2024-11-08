#include <queue>
#include <stack>
#include <chrono>
#include <iostream>
#include "MovementManager.hpp"

namespace hive
{
    MovementManager::MovementManager() {}

    // POWINIEN BRAĆ JAKO ARGUMENT POZYCJĘ PIONKA I TYP A NIE TILE
    std::set<Position> MovementManager::getAvailableMoves(char type, const Position &position) const
    {
        switch (type)
        {
        case 'G':
            return Grasshopper::getMoves(position, *this);
        case 'Q':
            return Queen::getMoves(position, *this);
        case 'B':
            return Beetle::getMoves(position, *this);
        case 'S':
            return Spider::getMoves(position, *this, emptyTiles);
        case 'A':
            return Ant::getMoves(position, *this, emptyTiles);
        default:
            return std::set<Position>();
        }
    }
}