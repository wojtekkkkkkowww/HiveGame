#pragma once

#include <set>
#include <deque>
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
        std::set<Action> getAvailableActions() const;
        bool isActionValid(const Action &action) const;

    private:
        void generateMoveActions(std::set<Action> &availableActions) const;
        void generatePlaceActions(std::set<Action> &availableActions) const;
        bool isPlaceActionValid(const Action &action) const;
        bool isMoveActionValid(const Action &action) const;
        void moveTile(Position position, Position newPosition);
        void placeTile(Position position, std::string type);
        
        Board &board;
        std::map<std::string, Player> &players;
        std::string &currentTurn;
        std::string &status;
        std::deque<Action> actions;
    };
}