#pragma once

#include <map>
#include <set>
#include <memory>
#include <list>
#include <stdexcept>

#include "Tile.hpp"
#include "MovementFunctions.hpp"

namespace hive
{
    class Board
    {
    public:
        Board();
        void resetBoard();
        Tile getTile(Position position);
        void removeTile(Position position);
        void addTile(Position position, Tile& tile);
        std::set<Position> getNeighbours(Position position);
        std::set<Position> getAvailableMoves(Tile tile);
        std::set<Position> getPlayerTiles(std::string color);
        void addEmptyTilesAroundBoard();
        bool isMoveBlocked(Position position, Position newPosition);
        bool isEmpty(Position neighbourPosition);
        bool isOccupiedByOpponent(Position pos, std::string color);
        int calculateNeighbours(Position position, std::string color);
        int getLevel(Position position);
        bool isQueenSurrounded(std::string color);
        std::map<Position, std::list<Tile>> boardTiles;
        std::set<Position> emptyTiles;


    private:

        bool isHiveConnectedAfterRemove(Position position);
        bool isDirectionBlocked(Position position, Position direction);
        bool isTouchingHiveAfterMove(Position position, Position newPosition);
    };
}