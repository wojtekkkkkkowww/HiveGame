// ActionHandler.cpp
#include "ActionHandler.hpp"
#include "Action.hpp"
#include <algorithm>

namespace hive
{
ActionHandler::ActionHandler(Board &board, Players &players, char &currentTurn,
                             std::string &status, std::stack<Action> &actions,
                             std::vector<Action> &availableActions)
    : board(board), players(players), currentTurn(currentTurn), status(status), actions(actions),
      availableActions(availableActions), articulationPointFinder(board)
{
}

bool ActionHandler::applyAction(const Action &action)
{
    if (!isActionValid(action))
    {
        return false;
    }

    actions.push(action);

    if (action.type == ActionType::PLACE)
    {
        placeTile(action.newPosition, action.tile_type);
    }
    else if (action.type == ActionType::MOVE)
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

    if (action.type == ActionType::MOVE)
    {
        moveTile(action.newPosition, action.position);
    }
    else if (action.type == ActionType::PLACE)
    {
        const Tile tile = board.getTile(action.newPosition);
        revertPlacedQueen(tile);
        board.removeTile(action.newPosition);
        char lastTurn = (currentTurn == 'W') ? 'B' : 'W';
        players[lastTurn].returnTile(action.tile_type);
    }
}

void ActionHandler::revertPlacedQueen(const Tile &tile)
{
    if (tile.type != 'Q')
    {
        return;
    }

    if (tile.color == 'W')
    {
        board.whiteQueen = invalidPosition;
    }
    else
    {
        board.blackQueen = invalidPosition;
    }
}

void ActionHandler::genAvailableActions()
{
    if (!players['W'].firstMove)
    {
        board.addEmptyTilesAroundBoard();
    }
    else
    {
        board.emptyTiles = {{0, 0}};
    }

    availableActions.clear();
    if (status == "PLAYING")
    {
        generatePlaceActions();
        generateMoveActions();
        if (availableActions.empty())
        {
            availableActions.push_back(makeWaitAction());
        }
    }
}

bool ActionHandler::isActionValid(const Action &action) const
{
    return std::find(availableActions.begin(), availableActions.end(), action) !=
           availableActions.end();
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
    if (!players[currentTurn].queenPlaced)
    {
        return;
    }

    articulationPointFinder.findArticulationPoints();

    std::set<Action> moves;
    for (const auto &position : board.getPlayerTiles(currentTurn))
    {
        const auto &tile = board.getTile(position);

        if (board.articulationPoints.find(position) != board.articulationPoints.end())
        {
            continue;
        }

        std::set<Position> availablePositions;
        board.getAvailableMoves(tile.type, position, availablePositions);
        for (const auto &newPosition : availablePositions)
        {
            Action action = makeMoveAction(position, newPosition);

            if (isMoveActionValid(action))
            {
                availableActions.push_back(action);
                moves.insert(action);
            }
        }
    }
}

bool ActionHandler::isMoveActionValid(const Action &action) const
{
    return !board.isMoveBlocked(action.position, action.newPosition);
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
    tile.placed = true;
    board.addTile(position, tile);
    if (type == 'Q')
    {
        players[currentTurn].queenPlaced = true;
        updateQueenPosition(tile, position);
    }
    // board.addEmptyTilesAroundBoard();
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

void ActionHandler::generatePlaceActions()
{
    std::set<char> availableTypes;
    getAvailableTypes(availableTypes);

    std::set<Position> avaliablePositions;

    for (const auto &position : board.emptyTiles)
    {
        if (isPlaceActionValid(position))
        {
            avaliablePositions.insert(position);
        }
    }

    for (const auto &type : availableTypes)
    {
        for (const auto &position : avaliablePositions)
        {
            availableActions.push_back(makePlaceAction(position, type));
        }
    }
}

void ActionHandler::getAvailableTypes(std::set<char> &availableTypes) const
{
    if (!players[currentTurn].queenPlaced && players[currentTurn].turnCounter >= 3)
    {
        availableTypes.insert('Q');
    }
    else
    {
        for (const auto &definition : pieceCatalog)
        {
            const auto type = definition.code;
            if (players[currentTurn].getTileCount(type) > 0)
            {
                availableTypes.insert(type);
            }
        }
    }
}
} // namespace hive
