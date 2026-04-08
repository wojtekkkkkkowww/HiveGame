#pragma once

#include "Position.hpp"
#include <string>

namespace hive
{
/*
 * @brief The Tile struct represents a single tile in the game.
 */
struct Tile
{
    Tile() = default;

    Tile(char type, char color) : color(color), type(type) {}

    Tile(const std::string &notation) : notation(notation), color(notation[0]), type(notation[1]) {}

    bool operator==(const Tile &other) const
    {
        return this->type == other.type && this->color == other.color &&
               this->position == other.position;
    }

    void setPosition(Position position) { this->position = position; }

    void setNotation(std::string notation) { this->notation = std::move(notation); }

    Position position = invalidPosition;
    std::string notation;
    char color;
    char type;
    bool placed = false;
};

} // namespace hive
