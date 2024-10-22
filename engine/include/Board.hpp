#pragma once

#include <map>
#include <set>
#include <memory>
#include <stdexcept>
#include "Tile.hpp"
#include "BaseBoard.hpp"
#include "MovementManager.hpp"

namespace hive
{
    class Board : public MovementManager
    {
    public:
        Board()
        {
            BaseBoard::resetBoard();
        }
    };
}