#ifndef PIECE_H
#define PIECE_H
#include <memory>
#include <set>

#define N {0,-1}
#define S {0,1}
#define NE {1,-1}
#define SE {1,0}
#define SW {-1,1}
#define NW {-1,0}

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
            return this->type == other.type ;
        }

        std::set<std::pair<int,int>> getNeighbours(); 
        void setPosition(std::pair<int,int> position) { this->position = position; }
        

        std::shared_ptr<HiveBoard> board;
        std::pair<int,int> position;
        TileType type;

    };

    inline std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
        return {lhs.first + rhs.first, lhs.second + rhs.second};
    }
} 

#endif 