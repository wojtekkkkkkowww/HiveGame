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

    const auto &availableActions = game.getAvailableActions();
    if (!availableActions.empty())
    {
        std::uniform_int_distribution<size_t> dist(0, availableActions.size() - 1);
        auto it = availableActions.begin();
        std::advance(it, dist(randomEngine));
        return *it;
    }

    return {};
}

std::optional<Action> RandomAIAlgorithm::winInOneMove(PlayerColor player)
{
    const auto &availableActions = game.getAvailableActions();
    for (const auto &action : availableActions)
    {
        game.applyValidAction(action);
        const auto playerWin =
            player == PlayerColor::White ? GameStatus::WhiteWins : GameStatus::BlackWins;
        if (game.isGameOver() && game.getGameStatus() == playerWin)
        {
            game.revertValidAction();
            return action;
        }
        game.revertValidAction();
    }

    return std::nullopt;
}
} // namespace hive
