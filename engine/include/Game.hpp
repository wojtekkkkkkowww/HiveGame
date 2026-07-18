#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>
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
    void startGameFromState(std::map<Position, std::deque<Tile>> tiles, char currentTurn);
    bool applyAction(const Action &action);
    void applyValidAction(const Action &action);
    bool applyAction(const std::string &actionString);
    void revertAction();
    void revertValidAction();
    void genAvailableActions();
    bool isGameOver() const;
    char getCurrentTurn() const;
    std::string getGameStatus() const;
    std::string getLastAction() const;
    std::size_t getPlyCount() const { return plyCount; }
    std::size_t getMaxPly() const { return maxPly; }

    std::vector<Action> avaliableActions;
    Board board;
    char currentTurn;
    Players players;
    std::string gameStatus = "PLAYING";

  protected:
    std::size_t plyCount = 0;
    std::size_t maxPly;
    ActionHandler actionHandler;
    TurnManager turnManager;
    ActionParser actionParser;
    std::stack<Action> actions;
    std::vector<std::string> actionStrings;
};

inline void Game::applyValidAction(const Action &action)
{
    actionHandler.applyValidAction(action);
    turnManager.nextTurn();
}

} // namespace hive
