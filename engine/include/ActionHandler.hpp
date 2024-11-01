#pragma once

#include <set>
#include <stack>
#include "Action.hpp"
#include "Board.hpp"
#include "Player.hpp"

namespace hive
{
    /*
    * @brief The ActionHandler class is responsible for applying and reverting actions.
    */
    class ActionHandler
    {
    public:
        ActionHandler(Board &board, std::map<char, Player*> &players, char &currentTurn, std::string &status, std::stack<Action> &actions);
        bool applyAction(Action action);
        void revertAction();
        void genAvailableActions();
        bool isActionValid(const Action &action) const;
        void reset();
        Action getLastAction() const;
        std::set<Action> getAvailableActions() const { return availableActions; }

    private:
        void revertPacedQueen(char tile_type);
        void generateMoveActions();
        void generatePlaceActions();
        bool isPlaceActionValid(const Action &action) const;
        bool isMoveActionValid(const Action &action) const;
        void moveTile(Position position, Position newPosition);
        void placeTile(Position position, char type);
        void updateQueenPosition(const Tile &tile, const Position &newPosition);

        Board &board;
        std::map<char, Player*> &players;
        char &currentTurn;
        std::string &status;
        std::stack<Action> &actions;
        std::set<Action> availableActions;
    };
}