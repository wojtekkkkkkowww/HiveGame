#include "Player.hpp"
#include <sstream>

hive::Player::Player(PlayerColor color) : color(color) { reset(); }

hive::Tile hive::Player::takeTile(PieceType type)
{
    auto &count = pieceCounters[hive::pieceIndex(type)];
    if (count == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    --count;

    std::ostringstream stream;
    stream << toChar(color) << toChar(type);
    if (type != PieceType::Queen)
    {
        stream << getPlacedPieceCount(type);
    }
    return Tile(stream.str());
}

void hive::Player::returnTile(PieceType type)
{
    if (type == PieceType::Queen)
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

int hive::Player::getPlacedPieceCount(PieceType type) const
{
    const auto index = hive::pieceIndex(type);
    return pieceCatalog[index].initialCount - pieceCounters[index];
}

int hive::Player::getTileCount(PieceType type) const
{
    return pieceCounters[hive::pieceIndex(type)];
}

hive::Player &hive::Players::operator[](PlayerColor color)
{
    return values[color == PlayerColor::White ? 0U : 1U];
}

const hive::Player &hive::Players::operator[](PlayerColor color) const
{
    return values[color == PlayerColor::White ? 0U : 1U];
}
