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
            moves.clear();

            for (auto &direction : directions)
            {
                Position newPos = position + direction;
                if (val.isEmpty(newPos) && !val.isDirectionBlocked(position, direction, 1))
                {
                    moves.insert(newPos);
                }
            }
            return moves;
        }

    };

}