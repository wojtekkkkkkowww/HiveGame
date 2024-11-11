#pragma once
#include <vector>
#include <memory>
#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "Heuristic.hpp"

namespace hive
{
    class AlphaBetaAI : public AIAlgorithm
    {
    public:
        AlphaBetaAI(Game &game);
        void addHeuristic(std::unique_ptr<Heuristic> heuristic, int weight);
        Action getNextMove() override;
        int NodesNumber = 0;

    private:
        static constexpr int maxDepth = 3;
        static constexpr const char *name = "AlphaBetaAI";

        std::vector<std::pair<std::unique_ptr<Heuristic>, int>> heuristics;
        int evaluate() const;
        char player;

        std::pair<int, Action> maxValue(int alpha, int beta, int depth);
        std::pair<int, Action> minValue(int alpha, int beta, int depth);
    };

}