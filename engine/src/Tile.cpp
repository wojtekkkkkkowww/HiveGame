#include "Tiles/Tile.h"

std::set<std::pair<int, int>> hge::Tile::getNeighbours(){
    
    std::set<std::pair<int, int>> directions = {
        {1, -1},   
        {1, 0},   
        {0, 1},   
        {-1, 1},  
        {-1, 0},  
        {0, -1}   
    };

    std::set<std::pair<int, int>> neighbours;

    for(const auto& dir : directions){
        auto [dx, dy] = dir;
        neighbours.insert(std::make_pair(position.first + dx, position.second + dy));
    }

    return neighbours;
}