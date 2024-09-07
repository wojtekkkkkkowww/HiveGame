#ifndef Player_H
#define Player_H
#include <vector>
#include "Tiles/Tile.h"

namespace hge {
    class Player {
    public:
        Player();
    
        std::vector<Tile> tiles; // to będzie shared_ptr
        void makeTiles();
    };
} 

#endif