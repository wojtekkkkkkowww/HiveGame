#pragma once

#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <set>

#include "Player.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include "Action.hpp"
#include "Position.hpp"

namespace hive
{
    enum class GameStatus
    {
        PLAYING,
        WHITE_WINS,
        BLACK_WINS,
        DRAW
    };

    class Game
    {
    public:
        Game();
        ~Game();

        void startNewGame();
        void applyAction(Action action);
        std::set<Action> getAvailableActions();
        std::string getCurrentTurn() { return currentTurn; }

        Board board;
        std::string currentTurn;
        std::map<std::string, Player> players;
        GameStatus status = GameStatus::PLAYING;

    private:
        Tile createTile(std::string type, std::string color);
        void switchTurn();
        void updateGameStatus();
        void moveTile(Position position, Position newPosition);
        void placeTile(Position position, std::string type);
        void generateMoveActions(std::set<hive::Action> &availableActions);
        void generatePlaceActions(std::set<hive::Action> &availableActions);
        bool isActionValid(const Action& action);
        bool isPlaceActionValid(const Action& action);
        bool isMoveActionValid(const Action& action);
    };
}