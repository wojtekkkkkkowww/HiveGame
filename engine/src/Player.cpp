#include "Player.hpp"
#include <sstream>

hive::Player::Player(char color) : color(color) { reset(); }

hive::Tile hive::Player::takeTile(char type)
{
    auto &count = pieceCounters[pieceIndex(type)];
    if (count == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    --count;

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
    ++pieceCounters[pieceIndex(type)];
}

void hive::Player::reset()
{
    for (std::size_t index = 0; index < pieceDefinitions.size(); ++index)
    {
        pieceCounters[index] = pieceDefinitions[index].initialCount;
    }
    queenPlaced = false;
    firstMove = true;
    turnCounter = 0;
}

int hive::Player::getUnplacedPieceNumber(char type) const
{
    const auto index = pieceIndex(type);
    return pieceDefinitions[index].initialCount - pieceCounters[index];
}

int hive::Player::getTileCount(char type) const { return pieceCounters[pieceIndex(type)]; }

std::size_t hive::Player::pieceIndex(char type)
{
    for (std::size_t index = 0; index < pieceDefinitions.size(); ++index)
    {
        if (pieceDefinitions[index].type == type)
        {
            return index;
        }
    }
    throw std::invalid_argument("Invalid piece type");
}

hive::Player &hive::Players::operator[](char color)
{
    if (color == 'W')
    {
        return values[0];
    }
    if (color == 'B')
    {
        return values[1];
    }
    throw std::invalid_argument("Invalid player color");
}

const hive::Player &hive::Players::operator[](char color) const
{
    if (color == 'W')
    {
        return values[0];
    }
    if (color == 'B')
    {
        return values[1];
    }
    throw std::invalid_argument("Invalid player color");
}
