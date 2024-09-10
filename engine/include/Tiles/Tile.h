#ifndef PIECE_H
#define PIECE_H
#include <memory>
#include <set>

namespace hge {
    enum class TileType {
        QUEEN,
        BEETLE,
        SPIDER,
        GRASSHOPPER,
        ANT,
        EMPTY
    };
    
    class HiveBoard;
    class Tile {
    public:
        Tile() {} 
        Tile(TileType type) : type(type) {}
        Tile(TileType type, std::shared_ptr<HiveBoard> board) : board(board), type(type) {}
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

        std::set<std::pair<int,int>> getNeighbours(); 
        void setPosition(std::pair<int,int> position) { this->position = position; }
        virtual std::set<std::pair<int,int>> getAvailableMoves() = 0;
        

        std::shared_ptr<HiveBoard> board;
        std::pair<int,int> position;
        TileType type;
    };
} 

#endif 