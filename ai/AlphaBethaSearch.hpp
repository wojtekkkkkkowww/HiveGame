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
        int NodesNumber = 0;

    private:
        std::mt19937 randomGenerator{std::random_device{}()};
        static constexpr int maxDepth = 4;
        static constexpr const char *name = "AlphaBetaAI";


        QueenSafty queenSafty;
        AttackOponentQueen attackOponentQueen;
        BlockedTiles blockedTiles;
        std::string playerString;

        std::vector<std::pair<Heuristic &, double>> heuristics = {
            //10 10 2.52577 
            {queenSafty, 10},
            {attackOponentQueen, 10},
            {blockedTiles, 2.52577},
        };

        double evaluate() const;
        char player;

        std::pair<double, Action> maxValue(double alpha, double beta, int depth);
        std::pair<double, Action> minValue(double alpha, double beta, int depth);
        
        char opponent(char player) const
        {
            return (player == 'W') ? 'B' : 'W';
        }
    };

}