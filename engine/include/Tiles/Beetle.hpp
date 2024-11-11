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
                if (beetleJumpUp(position + direction, beetle_level, val))
                {
                    int newBeetleLevel = val.getLevel(position + direction) + 1;
                    if (!val.isDirectionBlocked(position, direction, newBeetleLevel))
                    {
                        moves.insert(position + direction);
                    }

                    continue;
                }

                if (!val.isDirectionBlocked(position, direction, beetle_level))
                    moves.insert(position + direction);
            }

            return moves;
        }

    }

}