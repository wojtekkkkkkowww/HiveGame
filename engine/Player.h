#ifndef Player_H
#define Player_H
#include <vector>
#include "Pieces/Piece.h"

namespace hge {
    class Player {
    public:
        Player();
    
        std::vector<Piece> pieces; // to będzie shared_ptr
        void makePieces();
    };
} 

#endif