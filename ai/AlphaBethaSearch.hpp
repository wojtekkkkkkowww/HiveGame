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
        void setHeuristicWeights(std::vector<int> weights);
        Action getNextMove() override;
        int NodesNumber = 0;

    private:
        static constexpr int maxDepth = 3;
        static constexpr const char *name = "AlphaBetaAI";
        
        QueenAvailableMoves queenAvailableMoves;
        OpponentQueenAvailableMoves opponentQueenAvailableMoves;
        TilesOroundOpponentQueen tilesOroundOpponentQueen;
        TilesOroundQuuen tilesOroundQuuen;
        
        // QueenSafty queenSafty;
        // AttackOponentQueen attackOponentQueen;
        BlockedTiles blockedTiles;
        TilesValueHeuristic tilesValueHeuristic;
        WinLoseHeuristic winLoseHeuristic;

        // 6 42 51 1 1 10 
        std::vector<std::pair<Heuristic&, int>> heuristics = {
            {queenAvailableMoves, 6},
            {opponentQueenAvailableMoves, 42},
            {tilesOroundOpponentQueen, 51},
            {tilesOroundQuuen, 1},
            {blockedTiles, 1},
            {tilesValueHeuristic, 10},
            {winLoseHeuristic, 1},

        };

        int evaluate() const;
        char player;

        std::pair<int, Action> maxValue(int alpha, int beta, int depth);
        std::pair<int, Action> minValue(int alpha, int beta, int depth);
    };

}