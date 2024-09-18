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

    enum class PlayerColor {
        WHITE_PLAYER,
        BLACK_PLAYER
    };

    class Player {
    public:
        Player(std::shared_ptr<HiveBoard> board);
    
        std::vector<std::shared_ptr<Tile>> pieces; 
        std::shared_ptr<HiveBoard> board;
        std::shared_ptr<Tile> addNewTile(TileType type);
        void makeTiles();
        void placeTile(std::shared_ptr<Tile> tile, std::pair<int, int> position);
        bool ownsPiece(std::pair<int, int> position);

        /*
        potzebna będzie fnukcja w board
        króra sprawdzi dla danego gracza czy może postawić pionka
        */

    };
} 

#endif