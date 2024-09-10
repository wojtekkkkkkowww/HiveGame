#ifndef Beetle_H
#define Beetle_H
#include "Tile.h"

namespace hge {
    class Beetle : public Tile {
    public:
        Beetle() : Tile(TileType::BEETLE) {}
        Beetle(std::shared_ptr<HiveBoard> board) : Tile(TileType::BEETLE, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override {
            return std::set<std::pair<int, int>>();
        }

    };
} // namespace hge



#endif