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
    TurnManager(Board &board, Players &players, PlayerColor &currentTurn, GameStatus &status,
                std::size_t &plyCount, const std::size_t &maxPly)
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
    PlayerColor &currentTurn;
    GameStatus &gameStatus;
    std::size_t &plyCount;
    const std::size_t &maxPly;
};

inline void TurnManager::nextTurn()
{
    players[currentTurn].firstMove = false;
    players[currentTurn].turnCounter++;
    ++plyCount;
    currentTurn = opposite(currentTurn);
    updateGameStatus();
}

inline void TurnManager::revertTurn()
{
    currentTurn = opposite(currentTurn);
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
    const bool whiteQueenSurrounded = board.isQueenSurrounded(PlayerColor::White);
    const bool blackQueenSurrounded = board.isQueenSurrounded(PlayerColor::Black);

    if (whiteQueenSurrounded && blackQueenSurrounded)
    {
        gameStatus = GameStatus::Draw;
    }
    else if (whiteQueenSurrounded)
    {
        gameStatus = GameStatus::BlackWins;
    }
    else if (blackQueenSurrounded)
    {
        gameStatus = GameStatus::WhiteWins;
    }
    else if (plyCount >= maxPly)
    {
        gameStatus = GameStatus::Draw;
    }
    else
    {
        gameStatus = GameStatus::Playing;
    }
}
} // namespace hive
