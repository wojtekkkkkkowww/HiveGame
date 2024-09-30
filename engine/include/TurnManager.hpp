#pragma once

#include <string>
#include <map>
#include "Player.hpp"
#include "Board.hpp"

namespace hive
{
    class TurnManager
    {
    public:
        TurnManager(Board &board, std::map<std::string, Player> &players, std::string &currentTurn, std::string &gameStatus);

        void nextTurn();
        void updateGameStatus();
        void revertTurn();

    private:
        Board &board;
        std::map<std::string, Player> &players;
        std::string &currentTurn;
        std::string &gameStatus;
    };
}