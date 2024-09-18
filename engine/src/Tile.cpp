#include "Tiles/Tile.h"

std::set<std::pair<int, int>> hge::Tile::getNeighbours(){
    
    std::set<std::pair<int, int>> directions = {N,S,NE,SW,NW,SE};  
    std::set<std::pair<int, int>> neighbours;

    for(const auto& dir : directions){
        neighbours.insert(position + dir);
    }

    return neighbours;
}