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
        
        void startNewGame();
        void startGameFromState(std::map<Position, std::deque<Tile>> tiles, char currentTurn);
        bool applyAction(Action action);
        bool applyAction(const std::string& actionString);
        void revertAction();
        bool isGameOver() const;
        char getCurrentTurn() const;
        std::string getGameStatus() const;
        std::string getLastAction() const;
        std::set<Action> getAvailableActions() const;

        Board board;
        char currentTurn;
        std::map<char, Player*> players;
        std::string gameStatus = "PLAYING";

    protected:
        ActionHandler actionHandler;
        TurnManager turnManager;
        ActionParser actionParser;
        std::stack<Action> actions;
        std::string lastAction;
    };
}