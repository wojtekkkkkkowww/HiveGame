#pragma once

#include <map>
#include <set>
#include <memory>
#include <list>
#include <stdexcept>

#include "Tile.hpp"
#include "BaseBoard.hpp"
#include "MovementManager.hpp"
#include "MoveValidator.hpp"

namespace hive
{
    class Board : public MovementManager, public MoveValidator
    {
    public:
        Board()
        {
            BaseBoard::resetBoard();
        }
        Board &operator=(Board &&other) noexcept
        {
            if (this != &other)
            {
                MovementManager::operator=(std::move(other));
                MoveValidator::operator=(std::move(other));
            }
            return *this;
        }
    };
}