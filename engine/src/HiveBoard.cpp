#include "HiveBoard.h"

hge::HiveBoard::HiveBoard(){
    resetBoard();
}

void hge::HiveBoard::resetBoard(){
    EmptyTile tile;
    tiles.clear();
    emptyTiles.clear();
    addEmptyTile(std::make_pair(0,0));
}

void hge::HiveBoard::addEmptyTile(std::pair<int, int> position){
    EmptyTile tile;
    tile.position = position;
    emptyTiles[position] = tile;
}

void hge::HiveBoard::addTile(std::pair<int, int> position, std::shared_ptr<Tile> tile){
    tile->position = position;
    tiles[position] = tile;
}

bool hge::HiveBoard::isEmptyTile(std::pair<int, int> position){
    return emptyTiles.find(position) != emptyTiles.end();   
}
