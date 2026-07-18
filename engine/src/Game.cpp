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
    board.setBoardTiles(tiles);
    this->currentTurn = currentTurn;
    for (auto &[position, tiles] : tiles)
    {
        for (const auto &t : tiles)
        {
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

    gameStatus = "PLAYING";
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

    Action action = actionParser.stringToAction(actionString);

    if (actionHandler.applyAction(action))
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
    actionHandler.genAvailableActions();
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
