#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"

namespace hive
{

    AlphaBetaAI::AlphaBetaAI(Game &game) : AIAlgorithm(game)
    {
    }

    void AlphaBetaAI::setHeuristicWeights(std::vector<double> weights)
    {
        for (size_t i = 0; i < weights.size(); i++)
        {
            heuristics[i].second = weights[i];
        }
    }

    Action AlphaBetaAI::getNextMove()
    {

        player = game.getCurrentTurn();
        playerWins = (player == 'W' ? "WHITE_WINS" : "BLACK_WINS");
        double alpha = negative_infinity;
        double beta = infinity;

        Action bestMove = alphabetha(alpha, beta, maxDepth, 1).second;

        return bestMove;
    }

    double AlphaBetaAI::evaluate() const
    {
        double totalScore = 0.0;
        for (const auto &[heuristic, weight] : heuristics)
        {
            totalScore += weight * static_cast<double>(heuristic.evaluate(game, player));
        }
        return totalScore;
    }

    double AlphaBetaAI::calculateScore(int color) const
    {
        if (game.isGameOver())
        {
            if (game.getGameStatus() == playerWins)
                return (color == 1) ? infinity : negative_infinity;
            else if (game.getGameStatus() == "DRAW")
                return 0.0;
            else
                return (color != 1) ? infinity : negative_infinity;
        }

        return color * evaluate();
    }

    std::pair<double, Action> AlphaBetaAI::alphabetha(double alpha, double beta, int depth, int color)
    {
        if (game.isGameOver() || depth == 0)
        {
            return std::make_pair(calculateScore(color), Action());
        }

        game.genAvailableActions();
        auto actions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;

        std::pair<double, Action> best = std::make_pair(negative_infinity, actions.front());

        std::shuffle(actions.begin(), actions.end(), randomGenerator);

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            double v2 = -alphabetha(-beta, -alpha, depth - 1, -color).first;
            game.revertAction(actions, emptyTiles);

            if (v2 > best.first)
            {
                best = std::make_pair(v2, action);
                alpha = std::max(alpha, best.first);
            }

            if (best.first >= beta)
            {
                break;
            }
        }

        return best;
    }

}