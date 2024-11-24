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
        TurnManager(Board &board, std::map<char, Player*> &players, char &currentTurn, std::string &status)
        : board(board), players(players), currentTurn(currentTurn), gameStatus(status) {}

        inline void nextTurn();
        inline void updateGameStatus();
        inline void revertTurn();

    private:
        Board &board;
        std::map<char, Player*> &players;
        char &currentTurn;
        std::string &gameStatus;
    };

    inline void TurnManager::nextTurn()
    {
        players[currentTurn]->firstMove = false;
        players[currentTurn]->turnCounter++;
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        updateGameStatus();
    }

    inline void TurnManager::revertTurn()
    {
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        updateGameStatus();
        players[currentTurn]->turnCounter--;

        if (players[currentTurn]->turnCounter == 0)
        {
            players[currentTurn]->firstMove = true;
        }
    }

    inline void TurnManager::updateGameStatus()
    {
        bool whiteQueenSurrounded = board.isQueenSurrounded('W');
        bool blackQueenSurrounded = board.isQueenSurrounded('B');

        if (whiteQueenSurrounded && blackQueenSurrounded)
        {
            gameStatus = "DRAW";
        }
        else if (whiteQueenSurrounded)
        {
            gameStatus = "BLACK_WINS";
        }
        else if (blackQueenSurrounded)
        {
            gameStatus = "WHITE_WINS";
        }
        else
        {
            gameStatus = "PLAYING";
        }
    }
}