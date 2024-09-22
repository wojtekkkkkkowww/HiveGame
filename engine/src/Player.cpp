#include "Player.hpp"

hive::Player::Player(Color color) : color(color)
{
    pieceCounters = {
        {TileType::QUEEN, 1},
        {TileType::SPIDER, 2},
        {TileType::BEETLE, 2},
        {TileType::GRASSHOPPER, 3},
        {TileType::ANT, 3}};
}

hive::Tile hive::Player::takeTile(TileType type)
{
    if (pieceCounters[type] == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    pieceCounters[type]--;

    return Tile(type, color);
}

std::ostream &hive::operator<<(std::ostream &os, const Player &player)
{
    os << "Player piece counters:\n";
    for (const auto &counter : player.pieceCounters)
    {
        os << "TileType: " << static_cast<int>(counter.first) << " Count: " << counter.second << "\n";
    }
    return os;
}
