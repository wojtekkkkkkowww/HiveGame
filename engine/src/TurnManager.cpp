#include "TurnManager.hpp"
#include <iostream>

namespace hive
{
    TurnManager::TurnManager(Board &board, std::map<char, Player*> &players, char &currentTurn, std::string &status)
        : board(board), players(players), currentTurn(currentTurn), gameStatus(status) {}

    void TurnManager::nextTurn()
    {
        players[currentTurn]->firstMove = false;
        players[currentTurn]->turnCounter++;
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        updateGameStatus();
    }

    void TurnManager::revertTurn()
    {
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        updateGameStatus();
        players[currentTurn]->turnCounter--;

        if (players[currentTurn]->turnCounter == 0)
        {
            players[currentTurn]->firstMove = true;
        }
    }

    void TurnManager::updateGameStatus()
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