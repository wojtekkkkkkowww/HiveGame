#include "RandomAi.hpp"
#include <iterator>

namespace hive
{
RandomAIAlgorithm::RandomAIAlgorithm(Game &game) : AIAlgorithm(game)
{

    randomEngine.seed(std::random_device{}());
}

Action RandomAIAlgorithm::getNextMove()
{
    if (auto action = winInOneMove(game.getCurrentTurn()))
    {
        return action.value();
    }

    const auto &availableActions = game.avaliableActions;
    if (!availableActions.empty())
    {
        std::uniform_int_distribution<size_t> dist(0, availableActions.size() - 1);
        auto it = availableActions.begin();
        std::advance(it, dist(randomEngine));
        return *it;
    }

    return {};
}

std::optional<Action> RandomAIAlgorithm::winInOneMove(char player)
{
    auto availableActions = game.avaliableActions;
    for (const auto &action : availableActions)
    {
        game.applyValidAction(action);
        std::string playerString = player == 'W' ? "WHITE" : "BLACK";
        if (game.isGameOver() && game.getGameStatus() == playerString + "_WINS")
        {
            game.revertValidAction();

            return action;
        }
        game.revertValidAction();
    }

    return std::nullopt;
}
} // namespace hive
