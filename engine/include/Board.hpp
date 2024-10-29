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
    /** 
     * @brief The Board class is board with all necessary methods for managing the game board
     */
    class Board : public MovementManager
    {
    public:
        Board()
        {
            BaseBoard::resetBoard();
        }
    };
}