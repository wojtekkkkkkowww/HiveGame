#pragma once
#include "AiAlgorithm.hpp"
#include "Game.hpp"
#include "Heuristic.hpp"
#include <random>
#include <vector>

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

    int maxDepth = 6;
    std::vector<std::vector<Action>> actionBuffers =
        std::vector<std::vector<Action>>(static_cast<std::size_t>(maxDepth + 1));
    PlayerColor player = PlayerColor::White;
    GameStatus playerWins = GameStatus::WhiteWins;
};

} // namespace hive
