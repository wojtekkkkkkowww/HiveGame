#pragma once

#include <deque>
#include <expected>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Action.hpp"
#include "ActionHandler.hpp"
#include "ActionParser.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "Tile.hpp"
#include "TurnManager.hpp"

namespace hive
{

enum class ApplyActionError
{
    InvalidNotation,
    InvalidAction
};

constexpr std::string_view toString(ApplyActionError error) noexcept
{
    switch (error)
    {
    case ApplyActionError::InvalidNotation:
        return "INVALID_NOTATION";
    case ApplyActionError::InvalidAction:
        return "INVALID_ACTION";
    }
    return "UNKNOWN";
}

class Game
{
  public:
    explicit Game(std::size_t maxPly = 200);
    ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(Game &&) = delete;

    void startNewGame();
    void startGameFromState(std::map<Position, std::deque<Tile>> tiles, PlayerColor currentTurn);
    std::expected<void, ApplyActionError> applyActionExpected(const Action &action);
    std::expected<void, ApplyActionError> applyActionExpected(const std::string &actionString);
    bool applyAction(const Action &action);
    void applyValidAction(const Action &action);
    bool applyAction(const std::string &actionString);
    void revertAction();
    void revertValidAction();
    void genAvailableActions();
    bool isGameOver() const;
    bool canRevertAction() const noexcept;
    const std::vector<Action> &getAvailableActions() const noexcept;
    std::vector<Action> takeAvailableActions();
    Board &getBoard() noexcept;
    const Board &getBoard() const noexcept;
    Players &getPlayers() noexcept;
    const Players &getPlayers() const noexcept;
    const std::vector<std::string> &getActionHistory() const noexcept;

    PlayerColor getCurrentTurn() const noexcept { return currentTurn; }

    GameStatus getGameStatus() const noexcept { return gameStatus; }

    std::string getLastAction() const;

    std::size_t getPlyCount() const { return plyCount; }

    std::size_t getMaxPly() const { return maxPly; }

  private:
    std::vector<Action> availableActions;
    Board board;
    PlayerColor currentTurn;
    Players players;
    GameStatus gameStatus = GameStatus::Playing;
    std::size_t plyCount = 0;
    std::size_t maxPly;
    ActionHandler actionHandler;
    TurnManager turnManager;
    ActionParser actionParser;
    std::expected<void, ApplyActionError> applyActionExpected(const Action &action,
                                                              const std::string &actionString);
    std::stack<Action> actions;
    std::vector<std::string> actionStrings;
};

inline void Game::applyValidAction(const Action &action)
{
    actionHandler.applyValidAction(action);
    turnManager.nextTurn();
}

inline const std::vector<Action> &Game::getAvailableActions() const noexcept
{
    return availableActions;
}

inline std::vector<Action> Game::takeAvailableActions()
{
    auto extracted = std::move(availableActions);
    availableActions.clear();
    return extracted;
}

inline Board &Game::getBoard() noexcept { return board; }

inline const Board &Game::getBoard() const noexcept { return board; }

inline Players &Game::getPlayers() noexcept { return players; }

inline const Players &Game::getPlayers() const noexcept { return players; }

inline const std::vector<std::string> &Game::getActionHistory() const noexcept
{
    return actionStrings;
}

} // namespace hive
