#pragma once

#include <set>
#include <stack>
#include "Action.hpp"
#include "Board.hpp"
#include "Player.hpp"

namespace hive
{
    class ActionHandler
    {
    public:
        ActionHandler(Board &board, std::map<std::string, Player> &players, std::string &currentTurn, std::string &status);

        bool applyAction(Action action);
        void revertAction();
        void genAvailableActions();
        bool isActionValid(const Action &action) const;
        std::set<Action> getAvailableActions() const { return availableActions; }

    private:
        void generateMoveActions();
        void generatePlaceActions();
        bool isPlaceActionValid(const Action &action) const;
        bool isMoveActionValid(const Action &action) const;
        void moveTile(Position position, Position newPosition);
        void updateQueenPosition(const hive::Tile &tile, const hive::Position &newPosition);
        void placeTile(Position position, char type);

        Board &board;
        std::map<std::string, Player> &players;
        std::string &currentTurn;
        std::string &status;
        std::stack<Action> actions;
        std::set<Action> availableActions;
    };
}