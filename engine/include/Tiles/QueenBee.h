#ifndef QUEEN_BEE_H
#define QUEEN_BEE_H
#include "Tile.h"

namespace hge {
    class QueenBee : public Tile {
    public:
        QueenBee() : Tile(TileType::QUEEN) {}
        QueenBee(std::shared_ptr<HiveBoard> board) : Tile(TileType::QUEEN, board) {}
        std::set<std::pair<int, int>> getAvailableMoves() override {
            std::set<std::pair<int,int>> neighbours = getNeighbours();
            std::set<std::pair<int,int>> moves;

            for (auto& neighbour : neighbours) {
                if (!board->isEmptyTile(neighbour)) 
                    moves.insert(neighbour);
            }

            return moves;
        }
    };
} 



#endif