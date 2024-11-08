#pragma once
#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>

namespace hive
{

    /*
    BEETLE:
    Może się poruszać na sąsiednie pola na tym samym poziomie lub różnym o 1.
    */
    namespace Beetle
    {
        inline bool beetleJumpUp(const hive::Position &newPos, int beetle_level, const MoveValidator &val)
        {
            return val.getLevel(newPos) >= beetle_level;
        }

        inline std::set<Position> getMoves(Position position, const MoveValidator &val)
        {
            std::set<Position> moves;

            auto beetle_level = val.getLevel(position);

            for (const auto &direction : directions)
            {
                Position newPos = position + direction;
                if (beetleJumpUp(newPos, beetle_level, val))
                {
                    int newBeetleLevel = val.getLevel(newPos) + 1;
                    if (!val.isDirectionBlocked(position, direction, newBeetleLevel))
                    {
                        moves.insert(newPos);
                    }

                    continue;
                }

                if (!val.isDirectionBlocked(position, direction, beetle_level))
                    moves.insert(newPos);
            }

            return moves;
        }

    }

}