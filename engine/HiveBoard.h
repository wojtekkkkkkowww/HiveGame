#ifndef HiveBoard_H
#define HiveBoard_H

#include <map>
#include "Player.h"
#include "Tiles/Tile.h"

namespace hge {

    class HiveBoard {
    public:
        HiveBoard();
        std::map<std::pair<int,int>,Tile> tiles;
        std::map<std::pair<int,int>,Tile> emptyTiles;
        void resetBoard();
    };
    
} // namespace hge

#endif 