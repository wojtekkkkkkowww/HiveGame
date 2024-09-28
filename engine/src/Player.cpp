#include "Player.hpp"

hive::Player::Player(std::string color) : color(color)
{
    pieceCounters = {
        {"QUEEN", 1},
        {"SPIDER", 2},
        {"BEETLE", 2},
        {"GRASSHOPPER", 3},
        {"ANT", 3}};
}

hive::Tile hive::Player::takeTile(std::string type)
{
    if (pieceCounters[type] == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    pieceCounters[type]--;

    return Tile(type, color);
}
