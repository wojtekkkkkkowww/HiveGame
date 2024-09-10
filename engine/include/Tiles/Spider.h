#ifndef Spider_H
#define Spider_H
#include "Tile.h"

namespace hge {
    class Spider : public Tile {
    public:
        Spider() : Tile(TileType::SPIDER) {}
        Spider(std::shared_ptr<HiveBoard> board) : Tile(TileType::SPIDER, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override {
            return std::set<std::pair<int, int>>();
        }

    };
} 



#endif