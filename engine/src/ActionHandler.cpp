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

        actions.push_back(action);

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

        Action action = actions.back();
        actions.pop_back();

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

    std::set<Action> ActionHandler::getAvailableActions() const
    {
        std::set<Action> availableActions = {};
       
        if (status == "PLAYING")
        {
            try{
                generatePlaceActions(availableActions);
            }
            catch (std::exception &e)
            {
                std::cerr << "DUPA" << e.what() << std::endl;
            }
            
            try
            {
                generateMoveActions(availableActions);
            }
            catch (std::exception &e)
            {
                std::cerr << "KUPA" << e.what() << std::endl;
            }

            try{
            if (availableActions.empty())
            {
                WaitAction action;
                availableActions.insert(action);
            }}catch(std::exception &e)
            {
                std::cerr << "SIUP TO JA" << e.what() << std::endl;
            }

        }


        std::cerr << "Available actions: " << std::endl;
        for (auto actio : availableActions)
        {
            std::cerr << actio << std::endl;
        }
        return availableActions;
    }

    bool ActionHandler::isActionValid(const Action &action) const
    {
        std::set<Action> availableActions = getAvailableActions();
        std::cerr << "----------------" << std::endl;
        std::cerr << action << std::endl;

        return availableActions.find(action) != availableActions.end();
    }

    void ActionHandler::generateMoveActions(std::set<Action> &availableActions) const
    {
        for (const auto &position : board.getPlayerTiles(currentTurn))
        {
            try
            {
                auto tile = board.getTile(position);
                for (const auto &newPosition : board.getAvailableMoves(tile))
                {
                    MoveAction action(position, newPosition);
                    if (isMoveActionValid(action))
                    {
                        availableActions.insert(action);
                    }
                }
            }
            catch (std::exception &e)
            {
                std::cerr << "dupa" << e.what() << std::endl;
            }
        }
    }

    void ActionHandler::generatePlaceActions(std::set<Action> &availableActions) const
    {
        std::vector<std::string> types = {"ANT", "BEETLE", "GRASSHOPPER", "SPIDER", "QUEEN"};

        for (const auto &type : types)
        {
            if (players[currentTurn].getTileCount(type) > 0)
            {
                for (const auto &position : board.emptyTiles)
                {
                    PlaceAction action(position, type);
                    if (isPlaceActionValid(action))
                    {
                        availableActions.insert(action);
                    }
                }
            }
        }
    }

    bool ActionHandler::isPlaceActionValid(const Action &action) const
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

    bool ActionHandler::isMoveActionValid(const Action &action) const
    {
        std::cerr << "Checking move action" << std::endl;
        std::cerr << action.type << " " << action.position.x << " " << action.position.y << " " << action.newPosition.x << " " << action.newPosition.y << std::endl;
        try
        {
            if (!players[currentTurn].queenPlaced)
            {
                std::cerr << "Queen not placed" << std::endl;
                return false;
            }
        }
        catch (std::exception &e)
        {
            std::cerr << "że co " << e.what() << std::endl;
            return false;
        }

        if (board.isMoveBlocked(action.position, action.newPosition))
        {
            std::cerr << "Move blocked" << std::endl;
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