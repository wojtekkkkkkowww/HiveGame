#include "ActionHandler.hpp"
#include <iostream>

namespace hive
{
    ActionHandler::ActionHandler(Board &board, std::map<std::string, Player> &players, std::string &currentTurn, std::string &status)
        : board(board), players(players), currentTurn(currentTurn), status(status) {}

    void ActionHandler::applyAction(Action action)
    {
        if (!isActionValid(action))
        {
            std::cerr << "Invalid action" << std::endl;
            throw std::invalid_argument("Invalid action");
        }

        actions.push(action);

        if (action.type == "PLACE")
        {
            placeTile(action.position, action.tile_type);
        }
        else if (action.type == "MOVE")
        {
            moveTile(action.position, action.newPosition);
        }
        else if (action.type == "WAIT")
        {
            std::cerr << "SKIPPING TURN" << std::endl;
        }
    }

    void ActionHandler::revertAction()
    {
        if (actions.empty())
        {
            return;
        }

        Action action = actions.top();
        actions.pop();

        if (action.type == "MOVE")
        {
            moveTile(action.newPosition, action.position);
        }
        else if (action.type == "PLACE")
        {
            board.removeTile(action.position);
            players[currentTurn].returnTile(action.tile_type);
        }
    }

    std::set<Action> ActionHandler::getAvailableActions()
    {
        std::set<Action> availableActions = {};
        if (status == "PLAYING")
        {
            generatePlaceActions(availableActions);
            generateMoveActions(availableActions);
            if (availableActions.empty())
            {
                Action action({"WAIT"});
                availableActions.insert(action);
            }
        }
        return availableActions;
    }

    bool ActionHandler::isActionValid(const Action &action)
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

    void ActionHandler::generateMoveActions(std::set<Action> &availableActions)
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

    void ActionHandler::generatePlaceActions(std::set<Action> &availableActions)
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

    bool ActionHandler::isPlaceActionValid(const Action &action)
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

    bool ActionHandler::isMoveActionValid(const Action &action)
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

    void ActionHandler::moveTile(Position position, Position newPosition)
    {
        auto tile = board.getTile(position);
        board.removeTile(position);
        board.addTile(newPosition, tile);
    }

    void ActionHandler::placeTile(Position position, std::string type)
    {
        auto tile = players[currentTurn].takeTile(type);
        board.addTile(position, tile);
        if (type == "QUEEN")
        {
            players[currentTurn].queenPlaced = true;
            std::string color = (currentTurn == "WHITE") ? "White" : "Black";
            std::cerr << "\033[34m" << color << " queen placed\033[0m" << std::endl;
        }
    }
}