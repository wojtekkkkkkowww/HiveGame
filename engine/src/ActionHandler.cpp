#include "ActionHandler.hpp"
#include <iostream>

namespace hive
{
    ActionHandler::ActionHandler(Board &board, std::map<std::string, Player> &players, std::string &currentTurn, std::string &status)
        : board(board), players(players), currentTurn(currentTurn), status(status) {}

    bool ActionHandler::applyAction(Action action)
    {
        if (!isActionValid(action))
        {
            std::cerr << "Invalid action" << std::endl;
            return false;
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

        return true;
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

    void ActionHandler::genAvailableActions()
    {
        availableActions.clear();
        if (status == "PLAYING")
        {
            generatePlaceActions();
            generateMoveActions();

            if (availableActions.empty())
            {
                WaitAction action;
                availableActions.insert(action);
            }
        }

        std::cerr << "Available actions: " << std::endl;
        for (auto actio : availableActions)
        {
            std::cerr << actio << std::endl;
        }
    }

    bool ActionHandler::isActionValid(const Action &action) const
    {
        std::cerr << "----------------" << std::endl;
        std::cerr << action << std::endl;

        return availableActions.find(action) != availableActions.end();
    }

    void ActionHandler::generateMoveActions()
    {
        for (const auto &position : board.getPlayerTiles(currentTurn))
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
    }

    void ActionHandler::generatePlaceActions()
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
            {
                return false;
            }
        }

        if (players[currentTurn].getTileCount(action.tile_type) == 0)
        {
            return false;
        }

        if (!board.isOccupiedByOpponent(action.position, currentTurn) || players[currentTurn].firstMove)
        {
            return true;
        }
        return false;
    }

    bool ActionHandler::isMoveActionValid(const Action &action) const
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
        updateQueenPosition(tile, newPosition);
    }

    void ActionHandler::placeTile(Position position, std::string type)
    {
        auto tile = players[currentTurn].takeTile(type);
        board.addTile(position, tile);
        if (type == "QUEEN")
        {
            players[currentTurn].queenPlaced = true;
            updateQueenPosition(tile, position);
            std::string color = (currentTurn == "WHITE") ? "White" : "Black";
            std::cerr << "\033[34m" << color << " queen placed\033[0m" << std::endl;
        }
    }

    void ActionHandler::updateQueenPosition(hive::Tile &tile, const hive::Position &newPosition)
    {
        if (tile.color == "WHITE" && tile.type == "QUEEN")
        {
            board.whiteQueen = newPosition;
        }

        if (tile.color == "BLACK" && tile.type == "QUEEN")
        {
            board.blackQueen = newPosition;
        }
    }

}