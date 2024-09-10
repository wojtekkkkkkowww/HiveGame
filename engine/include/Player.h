#ifndef Player_H
#define Player_H

#include <vector>

#include "HiveBoard.h"
#include "Tiles/Tile.h"
#include "Tiles/Ant.h"
#include "Tiles/Beetle.h"
#include "Tiles/GrassHopper.h"
#include "Tiles/Spider.h"
#include "Tiles/QueenBee.h"
#include "Tiles/EmptyTile.h"



namespace hge {
    class Player {
    public:
        Player(std::shared_ptr<HiveBoard> board);
    
        std::vector<std::shared_ptr<Tile>> tiles; // to będzie shared_ptr
        std::shared_ptr<HiveBoard> board;
        void makeTiles();
        std::shared_ptr<Tile> addNewTile(TileType type);

    };
} 

#endif