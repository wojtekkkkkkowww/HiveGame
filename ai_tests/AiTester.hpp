#pragma once

#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "RandomAi.hpp"
#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"
#include "FileOperations.hpp"
#include <vector>
#include <future>
#include <string>

namespace hive
{
    class AiTester
    {
    public:
        AiTester();
        void SA();
        void testAI();
        void performPerftTest(int maxDepth);
        void runGameSimulation();
        void setAi(const std::string &arg1, const std::string &arg2);
        void performRandomMoves(int, int);
        std::vector<double> neighbourWeigths(std::vector<double> &weights);
        Action getNextMove(std::string aiType);
        
    private:
        Game game;
        AlphaBetaAI alphaBetaAi;
        RandomAIAlgorithm randomAi;

        std::string ai1Type;
        std::string ai2Type;

        void perft(int depth, long long &nodes);
        int playTestGame(char player, AIAlgorithm &ai1, AIAlgorithm &ai2, Game &game) const;
        int runParallelGames(std::vector<double> &weights1, std::vector<double> &weights2) const;
        int runParallelGames(std::vector<double> &weights);
        void randomWeights(std::vector<double> &weights);
    };
}
