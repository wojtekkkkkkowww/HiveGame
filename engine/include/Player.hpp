#pragma once

#include <memory>
#include <map>
#include <stdexcept>
#include <iostream>
#include <ostream>

#include "Tile.hpp"

namespace hive
{

    class Player
    {
    public:
        Player() : color(Color::WHITE) {}
        Player(Color color);
        Tile takeTile(TileType type);
        bool queenPlaced = false;
        bool firstMove = true;
        friend std::ostream &operator<<(std::ostream &os, const Player &player);

    private:
        Color color;
        std::map<TileType, int> pieceCounters;
    };
}