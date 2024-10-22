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
            const auto &tile = board.getTile(position);
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
        std::vector<char> types = {'A', 'B', 'G', 'S', 'Q'};

        for (const auto &type : types)
        {
            if (players[currentTurn].getTileCount(type) > 0)
            {
                for (const auto &position : board.getEmptyTiles())
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
        if (!players[currentTurn].queenPlaced && players[currentTurn].turnCounter >= 3)
        {
            if (action.tile_type != 'Q')
            {
                return false;
            }
        }

        if (players[currentTurn].getTileCount(action.tile_type) == 0)
        {
            return false;
        }

        if (board.isOccupiedByOpponent(action.position, currentTurn) && !players[currentTurn].firstMove)
        {
            return false;
        }

        return true;
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
        Tile tile = board.getTile(position);
        board.moveTile(position, newPosition);
        updateQueenPosition(tile, newPosition);
    }

    void ActionHandler::placeTile(Position position, char type)
    {
        auto tile = players[currentTurn].takeTile(type);
        board.addTile(position, tile);
        if (type == 'Q')
        {
            players[currentTurn].queenPlaced = true;
            updateQueenPosition(tile, position);
            std::string color = (currentTurn == "WHITE") ? "White" : "Black";
            std::cerr << "\033[34m" << color << " queen placed\033[0m" << std::endl;
        }
    }

    void ActionHandler::updateQueenPosition(const hive::Tile &tile, const hive::Position &newPosition)
    {
        if (tile.color == "WHITE" && tile.type == 'Q')
        {
            board.whiteQueen = newPosition;
        }

        if (tile.color == "BLACK" && tile.type == 'Q')
        {
            board.blackQueen = newPosition;
        }
    }

}