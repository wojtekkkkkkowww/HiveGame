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
        AIAlgorithm(Game& game)
            : game(game) {}
        virtual ~AIAlgorithm() = default;

        /**
         * @brief Pure virtual function to determine the next move.
         * 
         * @return Action The next move to be made by the AI.
         */
        virtual Action getNextMove() = 0;
    protected:
        Game& game;
    };
}