#pragma once

#include <set>
#include <stack>
#include "Action.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "ArticulationPoints.hpp"

namespace hive
{
    /*
     * @brief The ActionHandler class is responsible for applying and reverting actions.
     */
    class ActionHandler
    {
    public:
        ActionHandler(Board &board, std::map<char, Player *> &players, char &currentTurn, std::string &status, std::stack<Action> &actions, std::set<Action> &availableActions);
        bool applyAction(Action action);
        void revertAction();
        void genAvailableActions();
        bool isActionValid(const Action &action) const;
        void reset();
        Action getLastAction() const;

    private:
        void revertPacedQueen(char tile_type);
        void generateMoveActions();
        bool isMoveActionValid(const Action &action) const;
        void moveTile(Position position, Position newPosition);
        void placeTile(Position position, char type);
        void updateQueenPosition(const Tile &tile, const Position &newPosition);
        void generatePlaceActions();
        inline bool isPlaceActionValid(const Position &position) const;
        void getAvailableTypes(std::set<char> &availableTypes) const;

        Board &board;
        std::map<char, Player *> &players;
        char &currentTurn;
        std::string &status;
        std::stack<Action> &actions;
        std::set<Action> &availableActions; // consider not copying this
        ArticulationPointFinder articulationPointFinder;
        constexpr static std::array<char, 5> types = {'A', 'B', 'G', 'S', 'Q'};
    };

    inline bool ActionHandler::isPlaceActionValid(const Position &position) const
    {
        return !(board.isOccupiedByOpponent(position, currentTurn) && !players[currentTurn]->firstMove);
    }
}