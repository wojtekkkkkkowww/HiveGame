#include "Game.hpp"
#include <iostream>

namespace hive
{
    Game::Game() : currentTurn("WHITE")
    {
        players["WHITE"] = Player("WHITE");
        players["BLACK"] = Player("BLACK");
        startNewGame();
    }

    Game::~Game()
    {
    }

    void Game::applyAction(Action action)
    {
        if (!isActionValid(action))
        {
            std::cerr << "Invalid action" << std::endl;
            throw std::invalid_argument("Invalid action");
        }

        if (action.type == "PLACE")
        {
            placeTile(action.position, action.tile_type);
        }
        else if (action.type == "MOVE")
        {
            moveTile(action.position, action.newPosition);
        }else if(action.type == "WAIT")
        {
            std::cerr << "SKIPPING TURN" << std::endl;
        }

        switchTurn();
    }

    void Game::moveTile(Position position, Position newPosition)
    {
        auto tile = board.getTile(position);
        board.removeTile(position);
        board.addTile(newPosition, tile);
    }

    void Game::placeTile(Position position, std::string type)
    {
        auto tile = createTile(type, currentTurn);
        board.addTile(position, tile);
        if (type == "QUEEN")
        {
            players[currentTurn].queenPlaced = true;
            std::string color = (currentTurn == "WHITE") ? "White" : "Black";
            std::cerr << "\033[34m" << color << " queen placed\033[0m" << std::endl;
        }
    }

    /*
    niesamowicie nie efektowne, ale zadziała
    */
    bool Game::isActionValid(const Action &action)
    {
        std::set<Action> availableActions = getAvailableActions();
        std::cerr << "Available actions: " << std::endl;
        for (auto actio : availableActions)
        {
            std::cerr << actio << std::endl;
        }
        std::cerr << "----------------" << std::endl;
        std::cerr << action << std::endl;
        return availableActions.find(action) != availableActions.end();
    }

    bool Game::isPlaceActionValid(const Action &action)
    {
        if (!players[currentTurn].queenPlaced && players[currentTurn].turnCounter >= 4)
        {
            if (action.tile_type != "QUEEN")
                return false;
        }

        if (players[currentTurn].getTileCount(action.tile_type) == 0)
        {
            return false;
        }

        std::string opponent = (currentTurn == "WHITE") ? "BLACK" : "WHITE";
        return board.isOccupiedByOpponent(action.position, opponent) || players[currentTurn].firstMove;
    }

    bool Game::isMoveActionValid(const Action &action)
    {
        if (!players[currentTurn].queenPlaced)
        {
            return false;
        }

        if (board.isMoveBlocked(action.position, action.newPosition))
        {
            return false;
        }
        return true;
    }

    std::set<Action> Game::getAvailableActions()
    {
        std::set<Action> availableActions = {};
        if (status == GameStatus::PLAYING)
        {
            generatePlaceActions(availableActions);
            generateMoveActions(availableActions);
            if(availableActions.empty())
            {
                Action action({"WAIT"});
                availableActions.insert(action);
            }
        }
        return availableActions;
    }

    void Game::generateMoveActions(std::set<hive::Action> &availableActions)
    {
        for (const auto &position : board.getPlayerTiles(currentTurn))
        {
            auto tile = board.getTile(position);
            for (const auto &newPosition : board.getAvailableMoves(tile))
            {
                Action action({"MOVE", position, newPosition});
                if (isMoveActionValid({"MOVE", position, newPosition}))
                {
                    availableActions.insert(action);
                }
            }
        }
    }

    void Game::generatePlaceActions(std::set<hive::Action> &availableActions)
    {
        std::vector<std::string> types = {"ANT", "BEETLE", "GRASSHOPPER", "SPIDER", "QUEEN"};

        for (const auto &type : types)
        {
            if (players[currentTurn].getTileCount(type) > 0)
            {
                for (const auto &position : board.emptyTiles)
                {
                    Action action({"PLACE", position, type});
                    if (isPlaceActionValid(action))
                    {
                        availableActions.insert(action);
                    }
                }
            }
        }
    }

    void Game::startNewGame()
    {
        currentTurn = "WHITE";
        status = GameStatus::PLAYING;
        board.resetBoard();
    }

    Tile Game::createTile(std::string type, std::string color)
    {
        return players[color].takeTile(type);
    }

    void Game::switchTurn()
    {
        players[currentTurn].firstMove = false;
        players[currentTurn].turnCounter++;
        currentTurn = (currentTurn == "WHITE") ? "BLACK" : "WHITE";
        updateGameStatus();
        std::string turn_string = (currentTurn == "WHITE") ? "White" : "Black";
        std::cerr << "\033[32mCurrent turn: " << turn_string << "\033[0m" << std::endl;
    }

    void Game::updateGameStatus()
    {
        std::string opponent = (currentTurn == "WHITE") ? "BLACK" : "WHITE";
        if (board.isQueenSurrounded(currentTurn) && board.isQueenSurrounded(opponent))
        {
            status = GameStatus::DRAW;
        }
        else if (board.isQueenSurrounded(currentTurn))
        {
            if (currentTurn == "WHITE")
            {
                status = GameStatus::BLACK_WINS;
            }
            else
            {
                status = GameStatus::WHITE_WINS;
            }
        }
    }
}