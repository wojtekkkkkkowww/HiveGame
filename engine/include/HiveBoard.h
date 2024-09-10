#ifndef HiveBoard_H
#define HiveBoard_H

#include <map>
#include <memory>

#include "Tiles/Tile.h"
#include "Tiles/EmptyTile.h"

namespace hge {

    class HiveBoard {
    public:
        HiveBoard();
        std::map<std::pair<int,int>,std::shared_ptr<Tile>> tiles;
        std::map<std::pair<int,int>,EmptyTile> emptyTiles;
        void resetBoard();
        void addEmptyTile(std::pair<int,int> position);
        void addTile(std::pair<int,int> position, std::shared_ptr<Tile> tile);
        bool isEmptyTile(std::pair<int,int> position);
    };
    
} 

#endif 