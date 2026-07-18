#include "Player.hpp"
#include <sstream>

hive::Player::Player(char color) : color(color) { reset(); }

hive::Tile hive::Player::takeTile(char type)
{
    auto &count = pieceCounters[hive::pieceIndex(type)];
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
    return Tile(stream.str());
}

void hive::Player::returnTile(char type)
{
    if (type == 'Q')
    {
        queenPlaced = false;
    }
    ++pieceCounters[hive::pieceIndex(type)];
}

void hive::Player::reset()
{
    for (std::size_t index = 0; index < pieceCatalog.size(); ++index)
    {
        pieceCounters[index] = pieceCatalog[index].initialCount;
    }
    queenPlaced = false;
    firstMove = true;
    turnCounter = 0;
}

int hive::Player::getUnplacedPieceNumber(char type) const
{
    const auto index = hive::pieceIndex(type);
    return pieceCatalog[index].initialCount - pieceCounters[index];
}

int hive::Player::getTileCount(char type) const
{
    return pieceCounters[hive::pieceIndex(type)];
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
