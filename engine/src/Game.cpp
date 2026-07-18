// Game.cpp
#include <deque>

#include "Game.hpp"

namespace hive
{
Game::Game(std::size_t maxPly)
    : currentTurn('W'), maxPly(maxPly),
      actionHandler(board, players, currentTurn, gameStatus, actions, avaliableActions),
      turnManager(board, players, currentTurn, gameStatus, plyCount, this->maxPly),
      actionParser(board, currentTurn, players)
{
    if (maxPly == 0)
    {
        throw std::invalid_argument("Move limit must be greater than zero");
    }
    startNewGame();
}

void Game::startGameFromState(std::map<Position, std::deque<Tile>> tiles, char currentTurn)
{
    players['W'].reset();
    players['B'].reset();
    actionStrings.clear();
    plyCount = 0;
    gameStatus = "PLAYING";
    board.setBoardTiles(tiles);
    this->currentTurn = currentTurn;
    std::array<int, 2> placedPieces{};

    for (auto &[position, tiles] : tiles)
    {
        for (const auto &t : tiles)
        {
            players[t.color].takeTile(t.type);
            const auto playerIndex = t.color == 'W' ? 0U : 1U;
            ++placedPieces[playerIndex];

            if (t.color == 'W' && t.type == 'Q')
            {
                board.whiteQueen = position;
                players['W'].queenPlaced = true;
            }
            if (t.color == 'B' && t.type == 'Q')
            {
                board.blackQueen = position;
                players['B'].queenPlaced = true;
            }
        }
    }

    players['W'].firstMove = placedPieces[0] == 0;
    players['B'].firstMove = placedPieces[1] == 0;
    players['W'].turnCounter = placedPieces[0];
    players['B'].turnCounter = placedPieces[1];
    actionHandler.reset();
    turnManager.updateGameStatus();
    actionHandler.genAvailableActions();
}

bool Game::applyAction(const Action &action)
{
    const std::string actionString = actionParser.actionToString(action);

    if (actionHandler.applyAction(action))
    {
        actionStrings.push_back(actionString);
        turnManager.nextTurn();
        actionHandler.genAvailableActions();

        return true;
    }
    return false;
}

bool Game::applyAction(const std::string &actionString)
{

    const auto action = actionParser.stringToAction(actionString);
    if (!action)
    {
        return false;
    }

    if (actionHandler.applyAction(*action))
    {
        actionStrings.push_back(actionString);
        turnManager.nextTurn();
        actionHandler.genAvailableActions();

        return true;
    }

    return false;
}

void Game::revertAction()
{
    if (!actionStrings.empty())
    {
        actionStrings.pop_back();
    }
    actionHandler.revertAction();
    turnManager.revertTurn();

    actionHandler.genAvailableActions();
}

void Game::genAvailableActions() { actionHandler.genAvailableActions(); }

void Game::revertValidAction()
{
    actionHandler.revertAction();
    turnManager.revertTurn();
}

bool Game::isGameOver() const { return gameStatus != "PLAYING"; }

char Game::getCurrentTurn() const { return currentTurn; }

std::string Game::getGameStatus() const { return gameStatus; }

std::string Game::getLastAction() const { return actionStrings.back(); }

void Game::startNewGame()
{
    currentTurn = 'W';
    plyCount = 0;
    gameStatus = "PLAYING";
    board.resetBoard();
    players['W'].reset();
    players['B'].reset();
    actionHandler.reset();
}
} // namespace hive
