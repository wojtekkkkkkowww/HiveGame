#include "RandomAi.hpp"
#include <cstdlib>
#include <ctime>
#include <iterator>

namespace hive
{
    RandomAIAlgorithm::RandomAIAlgorithm(Game& game) : AIAlgorithm(game)
    {
        std::srand(std::time(nullptr)); // Seed for random number generation
    }

    Action RandomAIAlgorithm::getNextMove()
    {
        auto availableActions = game.getAvailableActions();
        if (!availableActions.empty())
        {
            auto it = availableActions.begin();
            std::advance(it, std::rand() % availableActions.size());
            return *it;
        }
        return Action(); // Return a default action if no actions are available
    }
}