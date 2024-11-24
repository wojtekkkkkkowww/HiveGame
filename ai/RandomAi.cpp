#include "RandomAi.hpp"
#include <iterator>

namespace hive
{
    RandomAIAlgorithm::RandomAIAlgorithm(Game &game) : AIAlgorithm(game, "Random AI")
    {

        randomEngine.seed(std::random_device{}());
    }

    Action RandomAIAlgorithm::getNextMove()
    {
        auto availableActions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;

        if (auto action = winInOneMove(game.getCurrentTurn()))
        {
            return action.value();
        }

        if (!availableActions.empty())
        {
            std::uniform_int_distribution<size_t> dist(0, availableActions.size() - 1);
            auto it = availableActions.begin();
            std::advance(it, dist(randomEngine));
            return *it;
        }

        return Action();
    }
}
