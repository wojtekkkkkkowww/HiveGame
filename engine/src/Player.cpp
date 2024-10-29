#include "Player.hpp"
#include <sstream>

std::map<char, int> hive::Player::initialPieceCounts = {
    {'Q', 1},
    {'S', 2},
    {'B', 2},
    {'G', 3},
    {'A', 3}};

hive::Player::Player(char color) : color(color)
{
    pieceCounters = initialPieceCounts;
}

hive::Tile hive::Player::takeTile(char type)
{
    if (pieceCounters[type] == 0)
    {
        throw std::invalid_argument("No more pieces of this type available");
    }
    pieceCounters[type]--;


    std::ostringstream os;
    if (type == 'Q')
    {
        os << color << type;
    }
    else
    {
        os << color << type << getUnplacedPieceNumber(type);
    }
    std::string notation = os.str();
//    std::cerr << "Taking tile " << notation << std::endl;
    tileNotations[type] = notation; 
    return Tile(notation);
}

void hive::Player::returnTile(char type)
{
    pieceCounters[type]++;
}

int hive::Player::getUnplacedPieceNumber(char type) const
{
    return initialPieceCounts.at(type) - pieceCounters.at(type);
}
