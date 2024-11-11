#pragma once
#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>

namespace hive
{
    /*
    Queen:
    Może się poruszać na sąsiednie pola
    */

    namespace Queen
    {

        inline std::set<Position> getMoves(Position position, const MoveValidator &val)
        {
            std::set<Position> moves;
            for (auto &direction : directions)
            {
                if (val.isEmpty(position + direction) && !val.isDirectionBlocked(position, direction, 1))
                {
                    moves.insert(position + direction);
                }
            }
            return moves;
        }

    };

}