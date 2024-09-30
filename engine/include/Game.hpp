#pragma once

#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <set>
#include <stack>
#include <string>

#include "Player.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include "Action.hpp"
#include "Position.hpp"
#include "ActionHandler.hpp"
#include "TurnManager.hpp"

namespace hive
{

    class Game
    {
    public:
        Game();
        ~Game();

        void startNewGame();
        void applyAction(Action action);
        void revertAction();
        std::set<Action> getAvailableActions();
        std::string getCurrentTurn() { return currentTurn; }
        std::string getGameStatus() { return gameStatus; }

        Board board;
        std::string currentTurn;
        std::map<std::string, Player> players;
        std::string gameStatus = "PLAYING";

    private:
        ActionHandler actionHandler;
        TurnManager turnManager;
    };
}