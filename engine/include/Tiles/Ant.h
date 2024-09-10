#ifndef Ant_H
#define Ant_H
#include "Tile.h"

namespace hge {
    class Ant : public Tile {
    public:
        Ant() : Tile(TileType::ANT) {}
        Ant(std::shared_ptr<HiveBoard> board) : Tile(TileType::ANT, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override {
            return std::set<std::pair<int, int>>();
        }
    };
}



#endif