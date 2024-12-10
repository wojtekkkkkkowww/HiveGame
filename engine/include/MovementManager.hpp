#pragma once

#include <set>
#include <utility>
#include <functional>
#include "Position.hpp"
#include "MoveValidator.hpp"

namespace hive
{
    /*
     * @brief The MovementManager class is responsible for generating possible moves for each tile type.
     */
    class MovementManager : public MoveValidator
    {
    public:
        MovementManager();
        std::set<Position> getAvailableMoves(char type, const Position &position);
        std::set<Position> getAntMoves(Position position);
        std::set<Position> getSpiderMoves(Position position);
        std::set<Position> getGrasshopperMoves(Position position) const;
        std::set<Position> getBeetleMoves(Position position) const;
        std::set<Position> getQueenMoves(Position position) const;

        std::map<Position,std::set<Position>> antMoves;    

    };

}