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
        std::string opponent = (currentTurn == "WHITE") ? "BLACK" : "WHITE";
        if (board.isQueenSurrounded(currentTurn) && board.isQueenSurrounded(opponent))
        {
            gameStatus = "DRAW";
        }
        else if (board.isQueenSurrounded(currentTurn))
        {
            if (currentTurn == "WHITE")
            {
                gameStatus = "BLACK_WINS";
            }
            else
            {
                gameStatus = "WHITE_WINS";
            }
        }
        else
        {
            gameStatus = "PLAYING";
        }
    }
}