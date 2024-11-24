#pragma once
#include "Game.hpp"
#include <optional>

namespace hive
{
    /**
     * @brief The AIAlgorithm class is an abstract class that represents an AI algorithm for playing the Hive game.
     */
    class AIAlgorithm
    {
    public:
        AIAlgorithm(Game& game, const std::string& name)
            : game(game), name(name)  {}
        virtual ~AIAlgorithm() = default;

        /**
         * @brief Pure virtual function to determine the next move.
         * 
         * @return Action The next move to be made by the AI.
         */
        virtual Action getNextMove() = 0;
        std::optional<Action> winInOneMove(char player);
        const std::string& getName() const { return name; }

    protected:
        Game& game;
        std::string name;
    };
}