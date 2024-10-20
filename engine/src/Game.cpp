#include <iostream>
#include <deque>

#include "Game.hpp"

namespace hive
{
    Game::Game()
        : currentTurn("WHITE"),
          actionHandler(board, players, currentTurn, gameStatus),
          turnManager(board, players, currentTurn, gameStatus)
    {
        players["WHITE"] = Player("WHITE");
        players["BLACK"] = Player("BLACK");
        startNewGame();
        actionHandler.genAvailableActions();
    }

    Game::~Game() {}

    bool Game::applyAction(Action action)
    {
        if (actionHandler.applyAction(action))
        {
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
            turnManager.nextTurn();
            actionHandler.genAvailableActions();
            return true;
        }
        return false;
    }

    void Game::revertAction()
    {
        actionHandler.revertAction();
        turnManager.revertTurn();
    }

    std::set<Action> Game::getAvailableActions() const
    {
        return actionHandler.getAvailableActions();
    }

    void Game::startNewGame()
    {
        currentTurn = "WHITE";
        gameStatus = "PLAYING";
        board.resetBoard();
    }
}
