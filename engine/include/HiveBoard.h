#ifndef HiveBoard_H
#define HiveBoard_H

#include <map>
#include <memory>
#include <list>

#include "Tiles/Piece.h"
#include "Tiles/EmptyTile.h"

namespace hge {

    class HiveBoard {
    public:
        HiveBoard();
        std::map<std::pair<int,int>,std::list<std::shared_ptr<Piece>>> pieces;
        std::map<std::pair<int,int>,EmptyTile> emptyTiles;
        void addEmptyTilesAroundBoard();
        bool isEmpty(std::pair<int, int> neighbourPosition);
        void resetBoard();
        void addEmptyTile(std::pair<int, int> position);
        void addPiece(std::pair<int, int> position, std::shared_ptr<Piece> piece);
    };
    
} 

#endif 