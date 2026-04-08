#include "Player.hpp"
#include <sstream>

std::map<char, int> hive::Player::initialPieceCounts = {
    {'Q', 1}, {'S', 2}, {'B', 2}, {'G', 3}, {'A', 3}};

hive::Player::Player(char color) : color(color) { reset(); }

hive::Tile hive::Player::takeTile(char type)
{
    if (pieceCounters[type] == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    pieceCounters[type]--;

    std::ostringstream stream;
    if (type == 'Q')
    {
        stream << color << type;
    }
    else
    {
        stream << color << type << getUnplacedPieceNumber(type);
    }
    return {stream.str()};
}

void hive::Player::returnTile(char type)
{
    if (type == 'Q')
    {
        queenPlaced = false;
    }
    pieceCounters[type]++;
}

void hive::Player::reset()
{
    for (const auto &[type, count] : initialPieceCounts)
    {
        pieceCounters[type] = count;
    }
    queenPlaced = false;
    firstMove = true;
    turnCounter = 0;
}

int hive::Player::getUnplacedPieceNumber(char type) const
{
    return initialPieceCounts.at(type) - pieceCounters.at(type);
}
