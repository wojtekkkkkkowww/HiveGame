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
        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        player = game.getCurrentTurn();
        int alpha = negative_infinity;
        int beta = infinity;
        NodesNumber = 0;

        // win in one move
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
                //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                //std::cout << "getNextMove: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

                return action;
            }
            game.revertAction(availableActions, emptyTiles);
        }

        // std::cout << "Branching factor: " << game.avaliableActions.size() << std::endl;
        // maxValue(alpha, beta, maxDepth).second;
        Action bestMove = maxValue(alpha, beta, maxDepth).second;
        //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        //std::cout << "getNextMove: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
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
        //   std::cout << "deep: " << depth << std::endl;
        NodesNumber += 1;

        //  std::cout << "ENTER MAX " << depth << std::endl;
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
            game.revertAction(actions,emptyTiles);
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

    std::pair<int, Action> AlphaBetaAI::minValue(int alpha, int beta, int depth)
    {
        NodesNumber += 1;
        //   std::cout << "ENTER MIN " << depth  << std::endl;
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
            game.revertAction(actions,emptyTiles);
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
