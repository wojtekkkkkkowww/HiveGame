#pragma once

#include "Piece.hpp"

namespace hge
{
    class Ant : public Piece
    {
    public:
        Ant(std::shared_ptr<HiveBoard> board) : Piece(TileType::ANT, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override
        {
            return std::set<std::pair<int, int>>();
        }
    };
}
