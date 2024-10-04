#include <iostream>
#include <deque>

#include "Game.hpp"

namespace hive
{
    Game::Game()
        : currentTurn("WHITE"),
          actionHandler(board, players, currentTurn,gameStatus),
          turnManager(board, players, currentTurn,gameStatus)
    {
        players["WHITE"] = Player("WHITE");
        players["BLACK"] = Player("BLACK");
        startNewGame();
    }

    Game::~Game() {}

    void Game::applyAction(Action action)
    {
        actionHandler.applyAction(action);
        turnManager.nextTurn();
    }

    void Game::revertAction()
    {
        actionHandler.revertAction();
        turnManager.revertTurn();
    }

    std::set<Action> Game::getAvailableActions() const
    {   try{
        return actionHandler.getAvailableActions();
        }
        catch(const std::exception &e)
        {
            std::cerr << "dupa" << e.what() << std::endl;
        
            return std::set<Action>();
        }
    }

    void Game::startNewGame()
    {
        currentTurn = "WHITE";
        gameStatus = "PLAYING";
        board.resetBoard();
    }
}
