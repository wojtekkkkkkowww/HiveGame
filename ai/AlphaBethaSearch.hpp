#pragma once
#include <vector>
#include <memory>
#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "Heuristic.hpp"

namespace hive {

    class AlphaBetaAI : public AIAlgorithm {
    public:
        AlphaBetaAI(Game& game);
        void addHeuristic(std::unique_ptr<Heuristic> heuristic, double weight);
        Action getNextMove() override;

    private:
        static constexpr int maxDepth = 2; 
        static constexpr const char* name = "AlphaBetaAI";

        std::vector<std::pair<std::unique_ptr<Heuristic>, double>> heuristics;

        double evaluate() const;        
        std::pair<double, Action> maxValue(double alpha, double beta, int depth);
        std::pair<double, Action> minValue(double alpha, double beta, int depth);
    };

} // namespace hive