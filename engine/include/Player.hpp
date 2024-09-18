#pragma once

#include <vector>

#include "HiveBoard.hpp"
#include "Tiles/Tile.hpp"
#include "Tiles/Ant.hpp"
#include "Tiles/Beetle.hpp"
#include "Tiles/GrassHopper.hpp"
#include "Tiles/Spider.hpp"
#include "Tiles/QueenBee.hpp"
#include "Tiles/EmptyTile.hpp"



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

