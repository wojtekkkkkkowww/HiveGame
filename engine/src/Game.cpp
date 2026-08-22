// Game.cpp
#include <deque>
#include <stdexcept>

#include "Game.hpp"

namespace hive
{
Game::Game(std::size_t maxPly)
    : currentTurn(PlayerColor::White), maxPly(maxPly),
      actionHandler(board, players, currentTurn, gameStatus, actions, availableActions),
      turnManager(board, players, currentTurn, gameStatus, plyCount, this->maxPly),
      actionParser(board, currentTurn, players)
{
    if (maxPly == 0)
    {
        throw std::invalid_argument("Move limit must be greater than zero");
    }
    startNewGame();
}

void Game::startGameFromState(std::map<Position, std::deque<Tile>> tiles, PlayerColor currentTurn)
{
    players[PlayerColor::White].reset();
    players[PlayerColor::Black].reset();
    actionStrings.clear();
    plyCount = 0;
    gameStatus = GameStatus::Playing;
    board.setBoardTiles(tiles);
    this->currentTurn = currentTurn;
    std::array<int, 2> placedPieces{};

    for (auto &[position, tiles] : tiles)
    {
        for (const auto &t : tiles)
        {
            players[t.color].takeTile(t.type);
            const auto playerIndex = t.color == PlayerColor::White ? 0U : 1U;
            ++placedPieces[playerIndex];

            if (t.color == PlayerColor::White && t.type == PieceType::Queen)
            {
                board.whiteQueen = position;
                players[PlayerColor::White].queenPlaced = true;
            }
            if (t.color == PlayerColor::Black && t.type == PieceType::Queen)
            {
                board.blackQueen = position;
                players[PlayerColor::Black].queenPlaced = true;
            }
        }
    }

    players[PlayerColor::White].firstMove = placedPieces[0] == 0;
    players[PlayerColor::Black].firstMove = placedPieces[1] == 0;
    players[PlayerColor::White].turnCounter = placedPieces[0];
    players[PlayerColor::Black].turnCounter = placedPieces[1];
    actionHandler.reset();
    turnManager.updateGameStatus();
    actionHandler.genAvailableActions();
}

bool Game::applyAction(const Action &action)
{
    return applyActionExpected(action).has_value();
}

std::expected<void, ApplyActionError> Game::applyActionExpected(const Action &action)
{
    return applyActionExpected(action, actionParser.actionToString(action));
}

bool Game::applyAction(const std::string &actionString)
{
    return applyActionExpected(actionString).has_value();
}

std::expected<void, ApplyActionError> Game::applyActionExpected(const std::string &actionString)
{
    const auto action = actionParser.parseAction(actionString);
    if (!action)
    {
        return std::unexpected(ApplyActionError::InvalidNotation);
    }
    return applyActionExpected(*action, actionString);
}

std::expected<void, ApplyActionError> Game::applyActionExpected(const Action &action,
                                                                const std::string &actionString)
{
    if (!actionHandler.applyAction(action))
    {
        return std::unexpected(ApplyActionError::InvalidAction);
    }
    actionStrings.push_back(actionString);
    turnManager.nextTurn();
    actionHandler.genAvailableActions();
    return {};
}

void Game::revertAction()
{
    if (!canRevertAction())
    {
        return;
    }
    if (!actionStrings.empty())
    {
        actionStrings.pop_back();
    }
    actionHandler.revertAction();
    turnManager.revertTurn();

    actionHandler.genAvailableActions();
}

void Game::genAvailableActions() { actionHandler.genAvailableActions(); }

void Game::revertValidAction()
{
    if (!canRevertAction())
    {
        return;
    }
    actionHandler.revertAction();
    turnManager.revertTurn();
}

bool Game::isGameOver() const { return gameStatus != GameStatus::Playing; }

bool Game::canRevertAction() const noexcept { return !actions.empty(); }

std::string Game::getLastAction() const
{
    if (actionStrings.empty())
    {
        throw std::logic_error("No actions recorded");
    }
    return actionStrings.back();
}

void Game::startNewGame()
{
    currentTurn = PlayerColor::White;
    plyCount = 0;
    gameStatus = GameStatus::Playing;
    actionStrings.clear();
    board.resetBoard();
    players[PlayerColor::White].reset();
    players[PlayerColor::Black].reset();
    actionHandler.reset();
}
} // namespace hive
