// ActionHandler.cpp
#include "ActionHandler.hpp"
#include "Action.hpp"
#include <algorithm>

namespace hive
{
ActionHandler::ActionHandler(Board &board, Players &players, PlayerColor &currentTurn,
                             GameStatus &status, std::stack<Action> &actions,
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

    applyValidAction(action);
    return true;
}

void ActionHandler::applyValidAction(const Action &action)
{
    actions.push(action);
    if (action.type == ActionType::PLACE)
    {
        placeTile(action.newPosition, action.pieceType);
    }
    else if (action.type == ActionType::MOVE)
    {
        moveTile(action.position, action.newPosition);
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

    if (action.type == ActionType::MOVE)
    {
        moveTile(action.newPosition, action.position);
    }
    else if (action.type == ActionType::PLACE)
    {
        const Tile tile = board.getTile(action.newPosition);
        revertPlacedQueen(tile);
        board.removeTile(action.newPosition);
        players[opposite(currentTurn)].returnTile(action.pieceType);
    }
}

void ActionHandler::revertPlacedQueen(const Tile &tile)
{
    if (tile.type != PieceType::Queen)
    {
        return;
    }

    if (tile.color == PlayerColor::White)
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
    if (!players[PlayerColor::White].firstMove)
    {
        board.addEmptyTilesAroundBoard();
    }
    else
    {
        board.emptyTiles = {{0, 0}};
    }

    availableActions.clear();
    if (status == GameStatus::Playing)
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

void ActionHandler::placeTile(Position position, PieceType type)
{
    auto tile = players[currentTurn].takeTile(type);
    tile.placed = true;
    board.addTile(position, tile);
    if (type == PieceType::Queen)
    {
        players[currentTurn].queenPlaced = true;
        updateQueenPosition(tile, position);
    }
    // board.addEmptyTilesAroundBoard();
}

void ActionHandler::updateQueenPosition(const hive::Tile &tile, const hive::Position &newPosition)
{
    if (tile.color == PlayerColor::White && tile.type == PieceType::Queen)
    {
        board.whiteQueen = newPosition;
    }

    if (tile.color == PlayerColor::Black && tile.type == PieceType::Queen)
    {
        board.blackQueen = newPosition;
    }
}

void ActionHandler::generatePlaceActions()
{
    const bool queenMustBePlaced =
        !players[currentTurn].queenPlaced && players[currentTurn].turnCounter >= 3;
    for (const auto &definition : pieceCatalog)
    {
        const auto type = definition.type;
        if ((queenMustBePlaced && type != PieceType::Queen) ||
            players[currentTurn].getTileCount(type) == 0)
        {
            continue;
        }

        for (const auto &position : board.emptyTiles)
        {
            if (isPlaceActionValid(position))
            {
                availableActions.push_back(makePlaceAction(position, type));
            }
        }
    }
}
} // namespace hive
