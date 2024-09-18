#ifndef Ant_H
#define Ant_H
#include "Piece.h"

namespace hge {
    class Ant : public Piece {
    public:
        Ant(std::shared_ptr<HiveBoard> board) : Piece(TileType::ANT, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override {
            return std::set<std::pair<int, int>>();
        }
    };
}



#endif