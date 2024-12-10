#include "MoveValidator.hpp"
#include <chrono>

namespace hive
{
    std::map<Position, std::vector<Position>> MoveValidator::neighboringDirections = {
        {NW, {W, NE}},
        {NE, {NW, E}},
        {E, {NE, SE}},
        {SE, {E, SW}},
        {SW, {SE, W}},
        {W, {NW, SW}}};

}