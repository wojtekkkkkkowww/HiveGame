#pragma once

#include <set>
#include <utility>
#include <functional>
#include "Position.hpp"
#include "MoveValidator.hpp"
#include "dfs.hpp"

namespace hive
{
    /*
    * @brief The MovementManager class is responsible for generating possible moves for each tile type.
    */
    class MovementManager : public MoveValidator
    {
    public:
        
        std::set<Position> getQueenBeeMoves(Position position) const;
        std::set<Position> getBeetleMoves(Position position) const;
        std::set<Position> getSpiderMoves(Position position) const;
        std::set<Position> getGrasshopperMoves(Position position) const;
        std::set<Position> getAntMoves(Position position) const;
        std::set<Position> getAvailableMoves(Tile tile) const;
    private:
        bool beatleJumpUp(const hive::Position &newPos, int beetle_level) const;
    };

}