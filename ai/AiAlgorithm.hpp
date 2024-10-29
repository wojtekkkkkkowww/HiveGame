#pragma once
#include "Game.hpp"

namespace hive
{
    class AIAlgorithm
    {
    public:
        AIAlgorithm(Game& game) : game(game) {}
        virtual ~AIAlgorithm() = default;

        /**
         * @brief Pure virtual function to determine the next move.
         * 
         * @return Action The next move to be made by the AI.
         */
        virtual Action getNextMove() = 0;

    protected:
        Game& game; ///< Reference to the game object.
    };
}