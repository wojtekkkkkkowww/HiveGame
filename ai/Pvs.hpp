#pragma once
#include <vector>
#include <memory>
#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "Heuristic.hpp"

namespace hive
{
    class PVS_AI : public AIAlgorithm
    {
    public:
        PVS_AI(Game &game);
        void setHeuristicWeights(std::vector<int> weights);
        Action getNextMove() override;
        int NodesNumber = 0;

    private:
        static constexpr int maxDepth = 3;
        static constexpr const char *name = "PVS";
        
        QueenAvailableMoves queenAvailableMoves;
        OpponentQueenAvailableMoves opponentQueenAvailableMoves;
        TilesOroundOpponentQueen tilesOroundOpponentQueen;
        TilesOroundQuuen tilesOroundQuuen;
        BlockedTiles blockedTiles;
        TilesValueHeuristic tilesValueHeuristic;
        WinLoseHeuristic winLoseHeuristic;

        std::vector<std::pair<Heuristic&, int>> heuristics = {
          //  {queenAvailableMoves, 1},
          //  {opponentQueenAvailableMoves, 1},
            {tilesOroundOpponentQueen, 100},
         //   {tilesOroundQuuen, 1},
         //   {blockedTiles, 1},
          //  {tilesValueHeuristic, 2},
            {winLoseHeuristic, 1},
        };

        int evaluate() const;
        char player;

        std::pair<int, Action> pvs(int alpha, int beta, int depth, bool isMax);
    };

}