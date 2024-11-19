#pragma once

#include <memory>
#include <set>
#include "Position.hpp"
#include <iostream>

namespace hive
{
    /*
    * @brief The Tile struct represents a single tile in the game.
    */
    struct Tile
    {
        Tile() {}

        Tile(char type, char color) : color(color), type(type) {}
        Tile(const std::string &notation) : notation(notation){
            this->type = notation[1];
            this->color = notation[0];
        }

        bool operator==(const Tile &other) const
        {
            return this->type == other.type && this->color == other.color && this->position == other.position;
        }

        void setPosition(Position position) { this->position = position; }
        void setNotation(std::string notation) { this->notation = notation; }

        Position position = invalidPosition;
        char color;
        std::string notation;
        char type; 
        bool placed = false;
    };

}
