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
    void AlphaBetaAI::setHeuristicWeights(std::vector<int> weights)
    {
        for (size_t i = 0; i < weights.size() - 1; i++)
        {
            heuristics[i].second = weights[i];
        }
    }

    Action AlphaBetaAI::getNextMove()
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

        Action bestMove = maxValue(alpha, beta, maxDepth).second;

        return bestMove;
    }

    int AlphaBetaAI::evaluate() const
    {
        int totalScore = 0.0;
        for (const auto &[heuristic, weight] : heuristics)
        {
            totalScore += weight * heuristic.evaluate(game, player);
        }
        return totalScore;
    }

    std::pair<int, Action> AlphaBetaAI::maxValue(int alpha, int beta, int depth)
    {

        NodesNumber += 1;

        if (game.isGameOver() || depth == 0)
        {
            return {evaluate(), Action()};
        }

        int v = negative_infinity;
        Action bestMove;

        game.genAvailableActions();
        auto actions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            int v2 = minValue(alpha, beta, depth - 1).first;
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

    std::pair<int, Action> AlphaBetaAI::minValue(int alpha, int beta, int depth)
    {
        NodesNumber += 1;

        if (game.isGameOver() || depth == 0)
        {
            return {evaluate(), Action()};
        }

        int v = infinity;
        Action bestMove;

        game.genAvailableActions();
        auto actions = game.avaliableActions;
        auto emptyTiles = game.board.emptyTiles;

        NodesNumber += actions.size();

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            int v2 = maxValue(alpha, beta, depth - 1).first;
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