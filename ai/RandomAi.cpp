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

    std::optional<Action> RandomAIAlgorithm::winInOneMove(char player)
    {
        auto availableActions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;
        for (const auto &action : availableActions)
        {
            game.applyValidAction(action);
            std::string playerString = player == 'W' ? "WHITE" : "BLACK";
            if (game.isGameOver() && game.getGameStatus() == playerString + "_WINS")
            {
                game.revertAction(availableActions, emptyTiles);

                return action;
            }
            game.revertAction(availableActions, emptyTiles);
        }

        return std::nullopt;
    }
}
