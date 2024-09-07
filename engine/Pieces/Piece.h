#ifndef PIECE_H
#define PIECE_H

namespace hge {
    enum class PieceType {
        QUEEN,
        BEETLE,
        SPIDER,
        GRASSHOPPER,
        ANT
    };

    class Piece {
    public:
        Piece(PieceType type) : type(type) {}
        virtual ~Piece() = default;

        bool operator==(const Piece& other) const {
           /*
           trzeba bedzie sprawdzic inne rzeczy jeszcze
           */
            return this->type == other.type ;
        }

        
    private:
        PieceType type;
    };
} 

#endif 