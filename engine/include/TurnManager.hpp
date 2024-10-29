#pragma once

#include <string>
#include <map>
#include "Player.hpp"
#include "Board.hpp"

namespace hive
{
    
    /*
    * @brief The TurnManager class is responsible for managing turns in the game.
    * It provides methods for changing the current turn, updating the game status, and reverting the last turn.
    */
    class TurnManager
    {
    public:
        TurnManager(Board &board, std::map<char, Player> &players, char &currentTurn, std::string &gameStatus);

        void nextTurn();
        void updateGameStatus();
        void revertTurn();

    private:
        Board &board;
        std::map<char, Player> &players;
        char &currentTurn;
        std::string &gameStatus;
    };
}