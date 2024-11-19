#include "Pvs.hpp"
#include "Heuristic.hpp"

namespace hive
{

    PVS_AI::PVS_AI(Game &game) : AIAlgorithm(game, name)
    {
    }

    void PVS_AI::setHeuristicWeights(std::vector<int> weights)
    {
        for (size_t i = 0; i < weights.size() - 1; i++)
        {
            heuristics[i].second = weights[i];
        }
    }

    Action PVS_AI::getNextMove()
    {
        player = game.getCurrentTurn();
        int alpha = negative_infinity;
        int beta = infinity;
        NodesNumber = 0;

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

        return pvs(alpha, beta, maxDepth, true).second;
    }

    int PVS_AI::evaluate() const
    {
        int totalScore = 0.0;
        for (const auto &[heuristic, weight] : heuristics)
        {
            totalScore += weight * heuristic.evaluate(game, player);
        }
        return totalScore;
    }

    std::pair<int, Action> PVS_AI::pvs(int alpha, int beta, int depth, bool isMax)
    {
        NodesNumber += 1;

        if (game.isGameOver() || depth == 0)
        {
            return {evaluate(), Action()};
        }

        int bestScore = (isMax) ? negative_infinity : infinity;
        Action bestAction;

        game.genAvailableActions();
        auto actions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;

        bool isFirstChild = true;

        for (const auto &action : actions)
        {
            game.applyValidAction(action);

            int currentScore;
            if (isFirstChild)
            {

                currentScore = (isMax) ? -pvs(-beta, -alpha, depth - 1, !isMax).first
                                       : -pvs(-alpha, -beta, depth - 1, !isMax).first;
            }
            else
            {

                currentScore = (isMax) ? -pvs(-alpha - 1, -alpha, depth - 1, !isMax).first
                                       : -pvs(-beta, -alpha, depth - 1, !isMax).first;

                if (alpha < currentScore && currentScore < beta)
                {
                    currentScore = (isMax) ? -pvs(-beta, -alpha, depth - 1, !isMax).first
                                           : -pvs(-alpha - 1, -alpha, depth - 1, !isMax).first;
                }
            }
            game.revertAction(actions, emptyTiles);

            if ((isMax && currentScore > bestScore) || (!isMax && currentScore < bestScore))
            {
                bestScore = currentScore;
                bestAction = action;
            }

            alpha = (isMax) ? std::max(alpha, bestScore) : alpha;

            if (alpha >= beta)
            {
                break;
            }

            isFirstChild = false;
        }

        return {bestScore, bestAction};
    }
}
