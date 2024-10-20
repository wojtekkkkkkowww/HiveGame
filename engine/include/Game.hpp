#pragma once

#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <set>
#include <deque>
#include <string>

#include "Player.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include "Action.hpp"
#include "Position.hpp"
#include "ActionHandler.hpp"
#include "TurnManager.hpp"
#include "ActionParser.hpp"

namespace hive
{

    class Game
    {
    public:
        Game();
        ~Game();
        
        /*
        Rozważ zamianę na 0/1 i enumy zamiast stringów
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        */


        void startNewGame();
        bool applyAction(Action action);
        bool applyAction(const std::string& actionString);
        void revertAction();
        std::set<Action> getAvailableActions() const;
        std::string getCurrentTurn() const { return currentTurn; }
        std::string getGameStatus() const { return gameStatus; }

        Board board;
        std::string currentTurn;
        std::map<std::string, Player> players;
        std::string gameStatus = "PLAYING";

    private:
        ActionHandler actionHandler;
        TurnManager turnManager;
        ActionParser actionParser;
    };
}