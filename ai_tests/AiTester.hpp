#pragma once

#include "AiAlgorithm.hpp"
#include "AlphaBethaSearch.hpp"
#include "Game.hpp"
#include "RandomAi.hpp"
#include <string>
#include <vector>

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
    Action getNextMove(const std::string &aiType);

  private:
    Game game;
    AlphaBetaAI alphaBetaAi;
    RandomAIAlgorithm randomAi;

    std::string ai1Type;
    std::string ai2Type;
    std::vector<std::vector<Action>> perftActionBuffers;

    void perft(int depth, long long &nodes);
    int playTestGame(PlayerColor player, AIAlgorithm &ai1, AIAlgorithm &ai2, Game &game) const;
    int runParallelGames(std::vector<double> &weights1, std::vector<double> &weights2) const;
    int runParallelGames(std::vector<double> &weights);
    void randomWeights(std::vector<double> &weights);
};
} // namespace hive
