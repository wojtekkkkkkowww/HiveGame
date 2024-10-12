#include "TurnManager.hpp"
#include <iostream>

namespace hive
{
    TurnManager::TurnManager(Board &board, std::map<std::string, Player> &players, std::string &currentTurn, std::string &status)
        : board(board), players(players), currentTurn(currentTurn), gameStatus(status) {}

    void TurnManager::nextTurn()
    {
        players[currentTurn].firstMove = false;
        players[currentTurn].turnCounter++;
        currentTurn = (currentTurn == "WHITE") ? "BLACK" : "WHITE";
        updateGameStatus();
        std::string turn_string = (currentTurn == "WHITE") ? "White" : "Black";
        std::cerr << "\033[32mCurrent turn: " << turn_string << "\033[0m" << std::endl;
    }

    void TurnManager::revertTurn()
    {
        currentTurn = (currentTurn == "WHITE") ? "BLACK" : "WHITE";
        updateGameStatus();
        players[currentTurn].turnCounter--;

        if (players[currentTurn].turnCounter == 0)
        {
            players[currentTurn].firstMove = true;
        }
    }

    void TurnManager::updateGameStatus()
    {
        bool whiteQueenSurrounded = board.isQueenSurrounded("WHITE");
        bool blackQueenSurrounded = board.isQueenSurrounded("BLACK");

        if (whiteQueenSurrounded && blackQueenSurrounded)
        {
            gameStatus = "DRAW";
            std::cerr << "\033[31mDRAW\033[0m" << std::endl;
        }
        else if (whiteQueenSurrounded)
        {
            gameStatus = "BLACK_WINS";
            std::cerr << "\033[31mBLACK WINS\033[0m" << std::endl;
        }
        else if (blackQueenSurrounded)
        {
            gameStatus = "WHITE_WINS";
            std::cerr << "\033[31mWHITE WINS\033[0m" << std::endl;
        }
        else
        {
            gameStatus = "PLAYING";
        }
    }
}