#include "Player.h"


hge::Player::Player(std::shared_ptr<HiveBoard> board) : board(board){
    makeTiles();
}

void hge::Player::makeTiles(){
    std::vector<std::shared_ptr<Tile>> tempTiles = {
        addNewTile(TileType::QUEEN),
        addNewTile(TileType::SPIDER),
        addNewTile(TileType::SPIDER),
        addNewTile(TileType::BEETLE),
        addNewTile(TileType::BEETLE),
        addNewTile(TileType::BEETLE),
        addNewTile(TileType::GRASSHOPPER),
        addNewTile(TileType::GRASSHOPPER),
        addNewTile(TileType::GRASSHOPPER),
        addNewTile(TileType::ANT),
        addNewTile(TileType::ANT)
    };
    tiles = std::move(tempTiles); // podoba mi sie bo prawdpopodobnie usuwa stare obiekty
}

std::shared_ptr<hge::Tile> hge::Player::addNewTile(TileType type)
{
    switch(type){
        case TileType::QUEEN:
            return std::make_shared<QueenBee>(board);
        case TileType::SPIDER:
            return std::make_shared<Spider>(board);
        case TileType::BEETLE:
            return std::make_shared<Beetle>(board);
        case TileType::GRASSHOPPER:
            return std::make_shared<GrassHopper>(board);
        case TileType::ANT:
            return std::make_shared<Ant>(board);
        default:
            return std::make_shared<EmptyTile>(); 
    }
}