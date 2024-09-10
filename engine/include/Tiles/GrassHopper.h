#ifndef Grass_Hopper_H
#define Grass_Hopper_H
#include "Tile.h"

namespace hge {
    class GrassHopper : public Tile {
    public:
        GrassHopper() : Tile(TileType::GRASSHOPPER) {}
        GrassHopper(std::shared_ptr<HiveBoard> board) : Tile(TileType::GRASSHOPPER, board) {}
        std::set<std::pair<int,int>> getAvailableMoves() override{ 
            return std::set<std::pair<int,int>>(); 
        }
    };
} 



#endif