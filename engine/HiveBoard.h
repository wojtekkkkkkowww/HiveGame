#ifndef HiveBoard_H
#define HiveBoard_H

#include <map>
#include "Player.h"
#include "Pieces/Piece.h"

namespace hge {

    class HiveBoard {
    public:
        HiveBoard();

        std::map<int,Piece> tiles;
        std::map<int,Piece> emptyTiles;
    };

} // namespace hge

#endif 