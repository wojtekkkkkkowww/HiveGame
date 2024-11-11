// Game.cpp
#include <iostream>
#include <deque>

#include "Game.hpp"

namespace hive
{
    Game::Game()
        : currentTurn('W'),
          actionHandler(board, players, currentTurn, gameStatus,actions, avaliableActions),
          turnManager(board, players, currentTurn, gameStatus),
          actionParser(board, currentTurn,players)
    {
        players['W'] = new Player('W');
        players['B'] = new Player('B');
        startNewGame();
    }

    Game::~Game()
    {
        delete players['W'];
        delete players['B'];
    }

    void Game::startGameFromState(std::map<Position, std::deque<Tile>> tiles, char currentTurn)
    {
        board.setBoardTiles(tiles);
        this->currentTurn = currentTurn;
        for (auto &[position, tiles] : tiles)
        {
            for (auto t : tiles)
            {
                if (t.color == 'W' && t.type == 'Q')
                {
                    board.whiteQueen = position;
                    players['W']->queenPlaced = true;
                }
                if (t.color == 'B' && t.type == 'Q')
                {
                    board.blackQueen = position;
                    players['B']->queenPlaced = true;
                }
            }
        }

        gameStatus = "PLAYING";
        actionHandler.genAvailableActions();
    }

    bool Game::applyAction(Action action)
    {
        actionStrings.push_back(actionParser.actionToString(action));
        std::vector<char> tile_types = {'Q','S','B','G','A'};

        if (actionHandler.applyAction(action))
        {
            turnManager.nextTurn();
            actionHandler.genAvailableActions();

            return true;
        }
        return false;
    }

    void Game::applyValidAction(Action action)
    {   
        actionHandler.applyAction(action);
        turnManager.nextTurn();
    }

    bool Game::applyAction(const std::string &actionString)
    {
        actionStrings.push_back(actionString);
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
        if(!actionStrings.empty())
            actionStrings.pop_back();
        actionHandler.revertAction();
        turnManager.revertTurn();
        
        actionHandler.genAvailableActions();
    }

    void Game::genAvailableActions()
    {   
        actionHandler.genAvailableActions();
    }

    void Game::revertAction(std::set<Action> actions)
    {
        if(!actionStrings.empty())
            actionStrings.pop_back();
    
        actionHandler.revertAction();
        turnManager.revertTurn();
        actionHandler.setAvailableActions(actions);
    }

    std::set<Action> Game::getAvailableActions() const
    {
        return avaliableActions;
    }

    bool Game::isGameOver() const
    {
        return gameStatus != "PLAYING";
    }

    char Game::getCurrentTurn() const
    {
        return currentTurn;
    }

    std::string Game::getGameStatus() const
    {
        return gameStatus;
    }

    std::string Game::getLastAction() const
    {
        return actionStrings.back();
    }

    void Game::startNewGame()
    {
        currentTurn = 'W';
        gameStatus = "PLAYING";
        board.resetBoard();
        players['W']->reset();
        players['B']->reset();
        actionHandler.reset();
    }
}
