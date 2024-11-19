#pragma once

#include <map>
#include <set>
#include <deque>
#include <list>
#include <vector>   
#include "Tile.hpp"
#include "Position.hpp"

namespace hive
{
    /**
     * @brief The BaseBoard class is for managing tiles on the board.
     * 
     */
    class BaseBoard
    {
    public:
        virtual ~BaseBoard() = default;
        
        void resetBoard();
        void setBoardTiles(std::map<Position, std::deque<Tile>>& tiles);
        void moveTile(Position position, Position newPosition);
        bool isEmpty(Position position) const;
        int getLevel(Position position) const;
        int calculateColoredNeighbours(Position position, char color) const;
        int calculateNeighbours(const Position &position) const;
        int getTileCount() const;
        void addTile(Position position, Tile tile);
        void removeTile(Position position);
        void addEmptyTilesAroundBoard();


        Position whiteQueen = invalidPosition;
        Position blackQueen = invalidPosition;

        Tile getTile(Position position) const;
        Tile getTileByNotation(std::string notation) const;
        std::list<Tile> getTiles() const;
        std::set<Position> getPlayerTiles(char color) const;
        std::vector<Position> getPositions() const;

        static std::set<Position> getNeighbours(Position position);
        


        std::map<Position, std::deque<Tile>> boardTiles;
        std::set<Position> emptyTiles;
        std::map<std::string,Position> positions;
    };
}