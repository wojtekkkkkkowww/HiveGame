#pragma once

#include "HiveBoard.h"
#include "Tile.h"

namespace hge {

class Piece : public Tile {
public:
    Piece(TileType type, std::shared_ptr<HiveBoard> board) : Tile(type, board) {}
    virtual std::set<std::pair<int, int>> getAvailableMoves() = 0;
};


}