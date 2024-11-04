#include "AlphaBethaSearch.hpp"

namespace hive
{

    AlphaBetaAI::AlphaBetaAI(Game &game) : AIAlgorithm(game, name) {}

    void AlphaBetaAI::addHeuristic(std::unique_ptr<Heuristic> heuristic, double weight)
    {
        heuristics.push_back(std::make_pair(std::move(heuristic), weight));
    }

    Action AlphaBetaAI::getNextMove()
    {
        double alpha = -std::numeric_limits<double>::infinity();
        double beta = std::numeric_limits<double>::infinity();

        const auto &[value, bestMove] = maxValue(alpha, beta, maxDepth);
        return bestMove;
    }

    double AlphaBetaAI::evaluate() const
    {
        double totalScore = 0.0;
        for (const auto &[heuristic, weight] : heuristics)
        {
            totalScore += weight * heuristic->evaluate(game);
        }
        return totalScore;
    }

    std::pair<double, Action> AlphaBetaAI::maxValue(double alpha, double beta, int depth)
    {
        //std::cout << "maxValue" << std::endl;
        //std::cout << "depth: " << depth << std::endl;
        
        if (game.isGameOver() || depth == 0){
            return {evaluate(), *(game.getAvailableActions().begin())};
        }
        double v = -std::numeric_limits<double>::infinity();
        Action bestMove;

        for (const auto &action : game.getAvailableActions())
        {

            //std::cout << "Max checks: " << action << std::endl;
            game.applyAction(action);
            double v2 = minValue(alpha, beta, depth - 1).first;
            game.revertAction();

            //std::cout << "v: " << v << " v2: " << v2 << std::endl;

            if (v2 > v)
            {
                v = v2;
                bestMove = action;
            }

            alpha = std::max(alpha, v);
            if (v >= beta)
                break; // Beta cutoff
        }
        //std::cout << "\e[0;35m maxValue: " << v << "\e[0m" << std::endl;
        return {v, bestMove};
    }

    std::pair<double, Action> AlphaBetaAI::minValue(double alpha, double beta, int depth)
    {
        //std::cout << "minValue" << std::endl;
        //std::cout << "depth: " << depth << std::endl;


        if (game.isGameOver() || depth == 0)
            return {evaluate(), *(game.getAvailableActions().begin())}; 

        double v = std::numeric_limits<double>::infinity();
        Action bestMove;

        for (const auto &action : game.getAvailableActions())
        {
            //std::cout << "Min checks: " << action << std::endl;
            game.applyAction(action);
            double v2 = maxValue(alpha, beta, depth - 1).first;
            game.revertAction();

            //std::cout << "v: " << v << " v2: " << v2 << std::endl;
            if (v2 < v)
            {
                v = v2;
                bestMove = action;
            }

            beta = std::min(beta, v);
            if (v <= alpha)
                break;
        }

        return {v, bestMove};
    }
}
