#pragma once

#include <memory>
#include <set>
#include "Position.hpp"
#include <iostream>

namespace hive
{
    class Tile
    {
    public:
        Tile() {}
        Tile(std::string type, std::string color) : color(color), type(type) {}

        bool operator==(const Tile &other) const
        {
            return this->type == other.type && this->color == other.color && this->position == other.position;
        }

        void setPosition(Position position) { this->position = position; }

        Position position;
        std::string color;
        std::string type;
    };

}
