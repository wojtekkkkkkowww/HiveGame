#pragma once

#include "Piece.hpp"

namespace hge
{
    class Beetle : public Piece
    {
    public:
        Beetle(std::shared_ptr<HiveBoard> board) : Piece(TileType::BEETLE, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override
        {
            return std::set<std::pair<int, int>>();
        }
    };
} 
