#include "AiAlgorithm.hpp"

namespace hive
{
    std::optional<Action> hive::AIAlgorithm::winInOneMove(char player)
    {
        auto availableActions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;
        for (const auto &action : availableActions)
        {
            if (action.type == "PLACE")
                continue;

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