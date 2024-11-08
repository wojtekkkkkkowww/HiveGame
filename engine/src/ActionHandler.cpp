// ActionHandler.cpp
#include "ActionHandler.hpp"
#include <iostream>
#include <chrono>

namespace hive
{
    ActionHandler::ActionHandler(Board &board, std::map<char, Player *> &players, char &currentTurn, std::string &status, std::stack<Action> &actions)
        : board(board), players(players), currentTurn(currentTurn), status(status), actions(actions) {}

    bool ActionHandler::applyAction(Action action)
    {
        if (!isActionValid(action))
        {
            return false;
        }

        actions.push(action);

        if (action.type == "PLACE")
        {
            placeTile(action.newPosition, action.tile_type);
        }
        else if (action.type == "MOVE")
        {
            moveTile(action.position, action.newPosition);
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
            revertPacedQueen(action.tile_type);
            board.removeTile(action.newPosition);
            char lastTurn = (currentTurn == 'W') ? 'B' : 'W';
            players[lastTurn]->returnTile(action.tile_type);
        }
        board.addEmptyTilesAroundBoard();
    }

    void ActionHandler::revertPacedQueen(char tile_type)
    {
        if (tile_type == 'Q')
        {
            if (currentTurn == 'W')
            {
                board.whiteQueen = invalidPosition;
            }
            else
            {
                board.blackQueen = invalidPosition;
            }
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
                availableActions.insert(WaitAction());
            }
        }
        //std ::cerr << "Available actions: " << availableActions.size() << std::endl;
    }

    bool ActionHandler::isActionValid(const Action &action) const
    {
        return availableActions.find(action) != availableActions.end();
    }

    void ActionHandler::reset()
    {
        while (!actions.empty())
        {
            actions.pop();
        }
        genAvailableActions();
    }

    Action ActionHandler::getLastAction() const
    {
        if (actions.empty())
        {
            throw std::invalid_argument("No actions to return");
        }
        return actions.top();
    }

    void ActionHandler::generateMoveActions()
    {
        for (const auto &position : board.getPlayerTiles(currentTurn))
        {
            const auto &tile = board.getTile(position);

            for (const auto &newPosition : board.getAvailableMoves(tile.type, position))
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
            if (players[currentTurn]->getTileCount(type) > 0)
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
        if (!players[currentTurn]->queenPlaced && players[currentTurn]->turnCounter >= 3)
        {
            if (action.tile_type != 'Q')
            {
                // std::cerr << "Queen not placed after 3 move" << std::endl;
                return false;
            }
        }

        if (players[currentTurn]->getTileCount(action.tile_type) == 0)
        {
            // std::cerr << "No tiles of this type" << std::endl;
            return false;
        }

        if (board.isOccupiedByOpponent(action.newPosition, currentTurn) && !players[currentTurn]->firstMove)
        {
            // std::cerr << "New position Occupied by opponent" << std::endl;
            return false;
        }

        return true;
    }

    bool ActionHandler::isMoveActionValid(const Action &action) const
    {
        if (!players[currentTurn]->queenPlaced)
        {
            // std::cerr << "Queen not placed" << std::endl;
            return false;
        }

        if (board.isMoveBlocked(action.position, action.newPosition))
        {
            // std::cerr << "Move blocked" << std::endl;
            return false;
        }
        // std::cerr << "Move correct" << std::endl;
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
        auto tile = players[currentTurn]->takeTile(type);
        tile.placed = true;
        board.addTile(position, tile);
        if (type == 'Q')
        {
            players[currentTurn]->queenPlaced = true;
            updateQueenPosition(tile, position);
            std::string color = (currentTurn == 'W') ? "White" : "Black";
        }
    }

    void ActionHandler::updateQueenPosition(const hive::Tile &tile, const hive::Position &newPosition)
    {
        if (tile.color == 'W' && tile.type == 'Q')
        {
            board.whiteQueen = newPosition;
        }

        if (tile.color == 'B' && tile.type == 'Q')
        {
            board.blackQueen = newPosition;
        }
    }
}