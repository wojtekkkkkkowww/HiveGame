#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"

namespace hive
{

    AlphaBetaAI::AlphaBetaAI(Game &game) : AIAlgorithm(game, name)
    {
    }

    /*
    SET WEIGHTS FOR HE without winLoseHeuristic
    */
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
        playerString = player == 'W' ? "WHITE" : "BLACK";
        double alpha = negative_infinity;
        double beta = infinity;
        NodesNumber = 0;

        auto availableActions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;
        for (const auto &action : availableActions)
        {
            if (action.type == "PLACE")
                continue;

            game.applyValidAction(action);
            if (game.isGameOver() && game.getGameStatus() == playerString + "_WINS")
            {
                game.revertAction(availableActions, emptyTiles);

                return action;
            }
            game.revertAction(availableActions, emptyTiles);
        }

        Action bestMove = maxValue(alpha, beta, maxDepth).second;

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

    std::pair<double, Action> AlphaBetaAI::maxValue(double alpha, double beta, int depth)
    {

        NodesNumber += 1;

        if (game.isGameOver() || depth == 0)
        {
            if(game.isGameOver())
            {
                if(game.getGameStatus() == playerString + "_WINS")
                    return {infinity, Action()};
                else if(game.getGameStatus() == "DRAW")
                    return {0.0, Action()};
                else 
                    return {negative_infinity, Action()};
            }
            return {evaluate(), Action()};
        }

        double v = negative_infinity;
        Action bestMove;

        game.genAvailableActions();
        auto actions = game.avaliableActions;
        std::shuffle(actions.begin(), actions.end(), randomGenerator);
        auto emptyTiles = game.board.emptyTiles;

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            double v2 = minValue(alpha, beta, depth - 1).first;
            game.revertAction(actions, emptyTiles);
            if (v2 > v)
            {
                v = v2;
                bestMove = action;
                alpha = std::max(alpha, v);
            }

            if (v >= beta)
            {

                break;
            }
        }
        return {v, bestMove};
    }

    std::pair<double, Action> AlphaBetaAI::minValue(double alpha, double beta, int depth)
    {
        NodesNumber += 1;

        if (game.isGameOver() || depth == 0)
        {
            if(game.isGameOver())
            {
                if(game.getGameStatus() == playerString + "_WINS")
                    return {infinity, Action()};
                else if(game.getGameStatus() == "DRAW")
                    return {0.0, Action()};
                else 
                    return {negative_infinity, Action()};
            }
            return {evaluate(), Action()};
        }

        double v = infinity;
        Action bestMove;

        game.genAvailableActions();
        auto actions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;
        std::shuffle(actions.begin(), actions.end(), randomGenerator);

        NodesNumber += actions.size();

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            double v2 = maxValue(alpha, beta, depth - 1).first;
            game.revertAction(actions, emptyTiles);
            if (v2 < v)
            {
                v = v2;

                bestMove = action;
                beta = std::min(beta, v);
            }

            if (v <= alpha)
            {

                break;
            }
        }
        return {v, bestMove};
    }
}