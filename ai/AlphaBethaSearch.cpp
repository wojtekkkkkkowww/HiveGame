#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"

namespace hive
{

    AlphaBetaAI::AlphaBetaAI(Game &game) : AIAlgorithm(game, name) {}

    void AlphaBetaAI::addHeuristic(std::unique_ptr<Heuristic> heuristic, double weight)
    {
        heuristics.push_back(std::make_pair(std::move(heuristic), weight));
    }

    Action AlphaBetaAI::getNextMove()
    {
        player = game.getCurrentTurn();
        double alpha = NEGATIVE_INFINITY;
        double beta = INFINITY;

        return maxValue(alpha, beta, maxDepth).second;
    }

    double AlphaBetaAI::evaluate() const
    {
        double totalScore = 0.0;
        for (const auto &[heuristic, weight] : heuristics)
        {
            totalScore += weight * heuristic->evaluate(game, player);
        }
        return totalScore;
    }

    std::pair<double, Action> AlphaBetaAI::maxValue(double alpha, double beta, int depth)
    {
      //  std::cout << "ENTER MAX " << depth << std::endl;
        if (game.isGameOver() || depth == 0)
        {
            if (game.isGameOver())
            {
                return {evaluate(), Action()};
            }

            return {evaluate(), *(game.getAvailableActions().begin())};
        }

        double v = NEGATIVE_INFINITY;
        Action bestMove;

        auto actions = game.getAvailableActions();
        for (const auto &action : actions)
        {
            game.applyAction(action);
            double v2 = minValue(alpha, beta, depth - 1).first;
            game.revertAction(actions);

            if (v2 > v)
            {
               // std::cout << "MAX v2: " << v2 << std::endl;
                v = v2;
                bestMove = action;
                alpha = std::max(alpha, v);
            }

            if (v >= beta)
            {
         //       std::cout << "beta puring" << std::endl;
                break;
            }
        }
        return {v, bestMove};
    }

    std::pair<double, Action> AlphaBetaAI::minValue(double alpha, double beta, int depth)
    {
     //   std::cout << "ENTER MIN " << depth  << std::endl;
        if (game.isGameOver() || depth == 0)
        {
            if (game.isGameOver())
            {
                return {evaluate(), Action()};
            }

            return {evaluate(), *(game.getAvailableActions().begin())};
        }

        double v = INFINITY;
        Action bestMove;

        auto actions = game.getAvailableActions();
        for (const auto &action : actions)
        {
            game.applyAction(action);
            double v2 = maxValue(alpha, beta, depth - 1).first;
            game.revertAction(actions);

            if (v2 < v)
            {
                v = v2;
         //       std::cout << "MIN v: " << v << std::endl;
                bestMove = action;
                beta = std::min(beta, v);
            }

            if (v <= alpha)
            {
           //     std::cout << "alpha puring" << std::endl;
                break;
            }
        }
        return {v, bestMove};
    }
}
