#include "TurnManager.hpp"
#include <iostream>

namespace hive
{
    TurnManager::TurnManager(Board &board, std::map<char, Player> &players, char &currentTurn, std::string &status)
        : board(board), players(players), currentTurn(currentTurn), gameStatus(status) {}

    void TurnManager::nextTurn()
    {
        players[currentTurn].firstMove = false;
        players[currentTurn].turnCounter++;
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        updateGameStatus();
        //std::string turn_string = (currentTurn == 'W') ? 'W' : 'B';
//        std::cerr << "\033[32mCurrent turn: " << currentTurn << "\033[0m" << std::endl;
    }

    void TurnManager::revertTurn()
    {
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
        updateGameStatus();
        players[currentTurn].turnCounter--;

        if (players[currentTurn].turnCounter == 0)
        {
            players[currentTurn].firstMove = true;
        }
    }

    void TurnManager::updateGameStatus()
    {
        bool whiteQueenSurrounded = board.isQueenSurrounded('W');
        bool blackQueenSurrounded = board.isQueenSurrounded('B');

        if (whiteQueenSurrounded && blackQueenSurrounded)
        {
            gameStatus = "DRAW";
//            std::cerr << "\033[31mDRAW\033[0m" << std::endl;
        }
        else if (whiteQueenSurrounded)
        {
            gameStatus = "BLACK_WINS";
//            std::cerr << "\033[31mBLACK WINS\033[0m" << std::endl;
        }
        else if (blackQueenSurrounded)
        {
            gameStatus = "WHITE_WINS";
//            std::cerr << "\033[31mWHITE WINS\033[0m" << std::endl;
        }
        else
        {
            gameStatus = "PLAYING";
        }
    }
}