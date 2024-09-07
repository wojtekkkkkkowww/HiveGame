#include "HiveBoard.h"

hge::HiveBoard::HiveBoard(){
    resetBoard();
}

void hge::HiveBoard::resetBoard(){
    Tile tile = Tile(TileType::EMPTY);
    tiles.clear();
    emptyTiles.clear();
    emptyTiles[std::make_pair(0,0)] = tile;
}