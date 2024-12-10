#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "Heuristic.hpp"

namespace hive
{
    class AlphaBetaAI : public AIAlgorithm
    {
    public:
        AlphaBetaAI(Game &game);
        void setHeuristicWeights(std::vector<double> weights);
        Action getNextMove() override;

    private:
        double evaluate() const;
        double calculateScore(int color) const;
        std::pair<double, Action> alphabetha(double alpha, double beta, int depth, int color);

        QueenSafty queenSafty;
        AttackOponentQueen attackOponentQueen;
        BlockedTiles blockedTiles;

        std::vector<std::pair<Heuristic &, double>> heuristics = {
            {queenSafty, 10},
            {attackOponentQueen, 10},
            {blockedTiles, 2.52577},
        };

        std::mt19937 randomGenerator{std::random_device{}()};

        int maxDepth = 4;
        char player;
        std::string playerWins;
    };

}