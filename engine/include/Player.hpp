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
        Player() : color("WHITE") {}
        Player(std::string color);
        Tile takeTile(char type);
        void returnTile(char type);
        int getTileCount(char type) { return pieceCounters[type]; } 
        bool queenPlaced = false;
        bool firstMove = true;
        int turnCounter = 0;

    private:
        std::string color;
        std::map<char, int> pieceCounters;
    };
}