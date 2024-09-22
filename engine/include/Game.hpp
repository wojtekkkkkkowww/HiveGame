#pragma once

#include <memory>
#include <vector>
#include <map>
#include <stdexcept>

#include "Player.hpp"
#include "Board.hpp"
#include "Tile.hpp"

namespace hive
{
    class Game
    {
    public:
        Game();
        ~Game();

        void startNewGame();
        void moveTile(std::pair<int, int> position, std::pair<int, int> newPosition);
        void placeTile(std::pair<int, int> position, TileType type);
        Color getCurrentTurn() { return currentTurn; }

        Board board;
        Color currentTurn;
        std::map<Color, Player> players;

    private:
        Tile createTile(TileType type, Color color);
        void switchTurn();
        void checkPositionAvailability(const Player& player, Color opponent, std::pair<int, int> position);
    };
}