#ifndef Grass_Hopper_H
#define Grass_Hopper_H
#include "Piece.hpp"

namespace hge
{
    class GrassHopper : public Piece
    {
    public:
        GrassHopper(std::shared_ptr<HiveBoard> board) : Piece(TileType::GRASSHOPPER, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override
        {
            return std::set<std::pair<int, int>>();
        }
    };
}

#endif