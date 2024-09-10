#include "Tile.h"

#ifndef EmptyTile_H
#define EmptyTile_H
#include "Tile.h"

namespace hge {
    class EmptyTile : public Tile {
    public:
        EmptyTile() : Tile(TileType::EMPTY) {}
        std::set<std::pair<int,int>> getAvailableMoves() override {
            return std::set<std::pair<int,int>>();
        }
    };
}



#endif