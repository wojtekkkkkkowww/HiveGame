#include <iostream>
#include <deque>

#include "Game.hpp"

namespace hive
{
    Game::Game()
        : currentTurn('W'),
          actionHandler(board, players, currentTurn, gameStatus),
          turnManager(board, players, currentTurn, gameStatus),
          actionParser(board, currentTurn)
    {
        players['W'] = Player('W');
        players['B'] = Player('B');
        startNewGame();
        actionHandler.genAvailableActions();
    }

    Game::~Game() {}

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

    bool Game::applyAction(Action action)
    {

        /*
        wymaga tego żeby było wykonane po akcji żeby znać daną notację brzmi prościej do rozwiązania
        ale wymaga wykonania przed akcją żeby board się zgadzał
        */
        lastAction = actionParser.actionToString(action);

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
        lastAction = actionString;
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
        return lastAction;
    }

    void Game::startNewGame()
    {
        currentTurn = 'W';
        gameStatus = "PLAYING";
        board.resetBoard();
    }
}
