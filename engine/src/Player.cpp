#include "Player.hpp"

hive::Player::Player(std::string color) : color(color)
{
    pieceCounters = {
        {'Q', 1},
        {'S', 2},
        {'B', 2},
        {'G', 3},
        {'A', 3}};
}

hive::Tile hive::Player::takeTile(char type)
{
    if (pieceCounters[type] == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    pieceCounters[type]--;

    return Tile(type, color);
}

void hive::Player::returnTile(char type)
{
    pieceCounters[type]++;
}
