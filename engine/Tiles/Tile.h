#ifndef PIECE_H
#define PIECE_H

namespace hge {
    enum class TileType {
        QUEEN,
        BEETLE,
        SPIDER,
        GRASSHOPPER,
        ANT,
        EMPTY
    };

    class Tile {
    public:
        Tile() : type(TileType::EMPTY) {} //domyślny konstruktor potrzebny do mapy
        Tile(TileType type) : type(type) {}
        virtual ~Tile() = default;

        bool operator==(const Tile& other) const {
           /*
           trzeba bedzie sprawdzic inne rzeczy jeszcze
           jakie ?
           czy tę samą pozycję mają być może
           */
            return this->type == other.type ;
        }
        /*
        To jest logiczne że Tile musi mieć wskaźnika na planszę
        no bo jak inaczej ruchy sprawdzi

        */

        
        TileType type;
    };
} 

#endif 