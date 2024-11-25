#pragma once

#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "RandomAi.hpp"
#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"
#include "DataBase.hpp"
#include "FileOperations.hpp"
#include "Pvs.hpp"
#include <vector>
#include <future>
#include <string>

namespace hive
{
    class AiTester
    {
    public:
        AiTester();
        void localSearch();
        void performPerftTest(int maxDepth);
        void runGameSimulation();
        void setAi(const std::string &arg1, const std::string &arg2);
        void performRandomMoves(int, int);
        Action getNextMove(std::string aiType);
        
    private:
        Game game;
        AlphaBetaAI alphaBetaAi;
        RandomAIAlgorithm randomAi;
        PVS_AI pvsAi;

        std::string ai1Type;
        std::string ai2Type;

        void perft(int depth, long long &nodes);
        int playTestGame(char player, AIAlgorithm &ai1, AIAlgorithm &ai2, Game &game) const;
        int runParallelGames(std::vector<int> &weights1, std::vector<int> &weights2) const;
        int runParallelGames(std::vector<int> &weights);
        void perturbWeights(std::vector<int> &weights, int maxChange);
    };
}
