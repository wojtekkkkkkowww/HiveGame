#pragma once

#include <map>
#include <set>
#include <deque>
#include <list>
#include <vector>   
#include <memory>
#include "Tile.hpp"
#include "Position.hpp"

namespace hive
{
    class BaseBoard
    {
    public:
        virtual ~BaseBoard() = default;
        
        void resetBoard();
        void moveTile(Position position, Position newPosition);
        bool isEmpty(Position position) const;
        int getLevel(Position position) const;
        int calculateNeighbours(Position position, std::string color) const;
        int getTileCount() const;
        void removeTile(Position position);
        void addTile(Position position, Tile tile);
        

        Tile getTile(Position position) const;
        std::set<Position> getEmptyTiles() const;
        std::list<Tile> getTiles() const;
        std::set<Position> getPlayerTiles(std::string color) const;

        static std::set<Position> getNeighbours(Position position);

        Position whiteQueen = invalidPosition;
        Position blackQueen = invalidPosition;


    protected:
        std::map<Position, std::deque<std::shared_ptr<Tile>>> boardTiles; 
        std::map<std::string,std::shared_ptr<Tile>> tilePositions;
        std::set<Position> emptyTiles;

        void addTile(Position position,std::shared_ptr<Tile> tile);
        void addEmptyTilesAroundBoard();
    };
}