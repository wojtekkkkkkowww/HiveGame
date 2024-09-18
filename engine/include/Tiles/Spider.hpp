#pragma once

#include "Piece.hpp"

namespace hge
{
    class Spider : public Piece
    {
    public:
        Spider(std::shared_ptr<HiveBoard> board) : Piece(TileType::SPIDER, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override
        {
            return std::set<std::pair<int, int>>();
        }
    };
}
