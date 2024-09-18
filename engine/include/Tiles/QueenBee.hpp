#pragma once

#include "Piece.hpp"

namespace hge
{
    class QueenBee : public Piece
    {
    public:
        QueenBee(std::shared_ptr<HiveBoard> board) : Piece(TileType::QUEEN, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override
        {
            std::set<std::pair<int, int>> neighbours = getNeighbours();
            std::set<std::pair<int, int>> moves;

            for (auto &neighbour : neighbours)
            {
                if (board->isEmpty(neighbour))
                    moves.insert(neighbour);
            }

            return moves;
        }
    };
}
