#pragma once

#include "Action.hpp"
#include "ArticulationPoints.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include <set>
#include <stack>

namespace hive
{
/*
 * @brief The ActionHandler class is responsible for applying and reverting actions.
 */
class ActionHandler
{
  public:
    ActionHandler(Board &board, Players &players, char &currentTurn,
                  std::string &status, std::stack<Action> &actions,
                  std::vector<Action> &availableActions);
    bool applyAction(const Action &action);
    void revertAction();
    void genAvailableActions();
    bool isActionValid(const Action &action) const;
    void reset();
    Action getLastAction() const;

  private:
    void revertPlacedQueen(const Tile &tile);
    void generateMoveActions();
    bool isMoveActionValid(const Action &action) const;
    void moveTile(Position position, Position newPosition);
    void placeTile(Position position, char type);
    void updateQueenPosition(const Tile &tile, const Position &newPosition);
    void generatePlaceActions();
    inline bool isPlaceActionValid(const Position &position) const;
    void getAvailableTypes(std::set<char> &availableTypes) const;

    Board &board;
    Players &players;
    char &currentTurn;
    std::string &status;
    std::stack<Action> &actions;
    std::vector<Action> &availableActions;
    ArticulationPointFinder articulationPointFinder;
    constexpr static std::array<char, 5> types = {'A', 'B', 'G', 'S', 'Q'};
};

inline bool ActionHandler::isPlaceActionValid(const Position &position) const
{
    return !board.isOccupiedByOpponent(position, currentTurn) || players[currentTurn].firstMove;
}
} // namespace hive
