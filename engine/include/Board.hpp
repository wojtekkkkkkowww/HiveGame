#pragma once

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
    Board() { BaseBoard::resetBoard(); }
};
} // namespace hive