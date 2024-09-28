#pragma once

#include <set>
#include <utility>
#include <functional>
#include "Position.hpp"
#include "BaseBoard.hpp"
#include "dfs.hpp"

namespace hive
{

    class MovementManager : public virtual BaseBoard
    {
    public:
        MovementManager& operator=(MovementManager&& other) noexcept {
            if (this != &other) {
                BaseBoard::operator=(std::move(other));
            }
            return *this;
        }

        std::set<Position> getQueenBeeMoves(Position position);
        std::set<Position> getBeetleMoves(Position position);
        std::set<Position> getSpiderMoves(Position position);
        std::set<Position> getGrasshopperMoves(Position position);
        std::set<Position> getAntMoves(Position position);
        std::set<Position> getAvailableMoves(Tile tile);
    };

}