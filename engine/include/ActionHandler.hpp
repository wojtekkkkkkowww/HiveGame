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

        void applyAction(Action action);
        void revertAction();
        std::set<Action> getAvailableActions();
        bool isActionValid(const Action &action);

    private:
        void generateMoveActions(std::set<Action> &availableActions);
        void generatePlaceActions(std::set<Action> &availableActions);
        bool isPlaceActionValid(const Action &action);
        bool isMoveActionValid(const Action &action);
        void moveTile(Position position, Position newPosition);
        void placeTile(Position position, std::string type);

        Board &board;
        std::map<std::string, Player> &players;
        std::string &currentTurn;
        std::string &status;
        std::stack<Action> actions;
    };
}