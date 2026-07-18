#pragma once

#include "Board.hpp"
#include "Player.hpp"
#include <map>
#include <string>

namespace hive
{
/*
 * @brief The TurnManager class is responsible for managing turns in the game.
 * It provides methods for changing the current turn, updating the game status, and reverting the
 * last turn.
 */
class TurnManager
{
  public:
    TurnManager(Board &board, Players &players, char &currentTurn,
                std::string &status, std::size_t &plyCount, const std::size_t &maxPly)
        : board(board), players(players), currentTurn(currentTurn), gameStatus(status),
          plyCount(plyCount), maxPly(maxPly)
    {
    }

    void nextTurn();
    void updateGameStatus();
    void revertTurn();

  private:
    Board &board;
    Players &players;
    char &currentTurn;
    std::string &gameStatus;
    std::size_t &plyCount;
    const std::size_t &maxPly;
};

inline void TurnManager::nextTurn()
{
    players[currentTurn].firstMove = false;
    players[currentTurn].turnCounter++;
    ++plyCount;
    currentTurn = (currentTurn == 'W') ? 'B' : 'W';
    updateGameStatus();
}

inline void TurnManager::revertTurn()
{
    currentTurn = (currentTurn == 'W') ? 'B' : 'W';
    players[currentTurn].turnCounter--;
    if (plyCount > 0)
    {
        --plyCount;
    }

    if (players[currentTurn].turnCounter == 0)
    {
        players[currentTurn].firstMove = true;
    }
    updateGameStatus();
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
    else if (plyCount >= maxPly)
    {
        gameStatus = "DRAW";
    }
    else
    {
        gameStatus = "PLAYING";
    }
}
} // namespace hive
