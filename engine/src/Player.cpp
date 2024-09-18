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
    pieces = std::move(tempTiles); // podoba mi sie bo prawdpopodobnie usuwa stare obiekty
}

void hge::Player::placeTile(std::shared_ptr<Tile> tile, std::pair<int, int> position)
{ /*
    tu będzie sporo warunków które trzeba spełnić żeby w ogóle można było postawić pionka
    */
   /*
   tutaj z poziomu gracza już coś można zrobić na przykład sprawdzić czyj to jest pionek
   */
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

bool hge::Player::ownsPiece(std::pair<int, int> position){
    for(auto tile : pieces){
        if(tile->position == position){
            return true;
        }
    }
    return false;
}

