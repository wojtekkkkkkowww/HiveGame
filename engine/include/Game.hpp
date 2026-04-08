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
    Game();
    ~Game();

    void startNewGame();
    void startGameFromState(std::map<Position, std::deque<Tile>> tiles, char currentTurn);
    bool applyAction(const Action &action);
    void applyValidAction(const Action &action);
    bool applyAction(const std::string &actionString);
    void revertAction(std::vector<Action> &actions, std::set<Position> &emptyTiles);
    void revertAction();
    void genAvailableActions();
    bool isGameOver() const;
    char getCurrentTurn() const;
    std::string getGameStatus() const;
    std::string getLastAction() const;

    std::vector<Action> avaliableActions;
    Board board;
    char currentTurn;
    std::map<char, Player *> players; // bez sensu
    std::string gameStatus = "PLAYING";

  protected:
    ActionHandler actionHandler;
    TurnManager turnManager;
    ActionParser actionParser;
    std::stack<Action> actions;
    std::vector<std::string> actionStrings;
};

inline void Game::applyValidAction(const Action &action)
{
    actionHandler.applyAction(action);
    turnManager.nextTurn();
}

} // namespace hive