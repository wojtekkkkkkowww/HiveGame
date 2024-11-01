#pragma once

#include <memory>
#include <map>
#include <stdexcept>
#include <iostream>
#include <ostream>

#include "Tile.hpp"

namespace hive
{
    /*
    * @brief The Player class represents a player in the game.
    * Each player has a color and a specyfic number of tiles.
    * Take function is used to take a tile from the player's pool.
    */
    class Player
    {
    public:
        Player() : color('W') {}
        Player(char color);
        Tile takeTile(char type);
        
        void returnTile(char type);
        void reset();
        int getTileCount(char type) { return pieceCounters[type]; } // ilość pozoostałych pionków
        int getUnplacedPieceNumber(char type) const;
        
        bool queenPlaced = false;
        bool firstMove = true;
        int turnCounter = 0;

    private:
        const char color;
        std::map<char, int> pieceCounters;        
        static std::map<char, int> initialPieceCounts;
    };
}