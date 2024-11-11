#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"

namespace hive
{

    AlphaBetaAI::AlphaBetaAI(Game &game) : AIAlgorithm(game, name)
    {
        addHeuristic(std::make_unique<PieceCountHeuristic>(), 1.0);
        addHeuristic(std::make_unique<TilesOroundOpponentQueen>(), 10.0);
        addHeuristic(std::make_unique<TilesValueHeuristic>(), 1.0);
        addHeuristic(std::make_unique<QueenAvailableMoves>(), 1.0);
        addHeuristic(std::make_unique<WinHeuristic>(), 1.0);
        addHeuristic(std::make_unique<LoseHeuristic>(), 1.0);
    
    }

    void AlphaBetaAI::addHeuristic(std::unique_ptr<Heuristic> heuristic, int weight)
    {
        heuristics.push_back(std::make_pair(std::move(heuristic), weight));
    }

    Action AlphaBetaAI::getNextMove()
    {
        player = game.getCurrentTurn();
        int alpha = negative_infinity;
        int beta = infinity;
        NodesNumber = 0;

        //win in one move
        auto availableActions = game.getAvailableActions();
        std::cout<<"checking win in one move"<<std::endl;
        for (const auto &action : availableActions)
        {
            if (action.type == "PLACE")
                continue;

            game.applyValidAction(action);
            std::string playerString = player == 'W' ? "WHITE" : "BLACK";
            if (game.isGameOver() && game.getGameStatus() == playerString + "_WINS")
            {
                game.revertAction(availableActions);
                std::cout << "Win in one move Detected" << std::endl;
                return action;
            }
            game.revertAction(availableActions);
        }

        std::cout << "Branching factor: " << game.getAvailableActions().size() << std::endl;
        return maxValue(alpha, beta, maxDepth).second;
    }

    int AlphaBetaAI::evaluate() const
    {
        int totalScore = 0.0;
        for (const auto &[heuristic, weight] : heuristics)
        {
            totalScore += weight * heuristic->evaluate(game, player);
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
        auto actions = game.getAvailableActions();

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            int v2 = minValue(alpha, beta, depth - 1).first;
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
        auto actions = game.getAvailableActions();

        NodesNumber += actions.size();

        for (const auto &action : actions)
        {
            game.applyValidAction(action);
            int v2 = maxValue(alpha, beta, depth - 1).first;
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
