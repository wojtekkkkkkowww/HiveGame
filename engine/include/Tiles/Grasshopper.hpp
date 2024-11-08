#pragma once
#include "MoveValidator.hpp"
#include "Position.hpp"
#include <set>
#include <stack>
#include <chrono>

namespace hive
{

    /*
    GRASSHOPPER:
    Przeskakuje przez figury na puste pole za nimi. W dowolnym kierunku.
    Musi przeskoczyć przez co najmniej jedną figurę.
    */

    namespace Grasshopper
    {

        inline std::set<Position> getMoves(Position position, const MoveValidator &val)
        {
            std::set<Position> moves;

            for (auto &direction : directions)
            {
                Position current = position;

                // nie może skakać na puste pola obok siebie
                if (val.isEmpty(current + direction))
                {
                    continue;
                }

                while (!val.isEmpty(current))
                {
                    current = current + direction;
                }

                moves.insert(current);
            }

            return moves;
        }
    };

}