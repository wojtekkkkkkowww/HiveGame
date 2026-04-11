#pragma once

#include "Position.hpp"
#include "Tile.hpp"
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace hive
{
/**
 * @brief The Board class is board with all necessary methods for managing the game board
 */
class Board
{
  public:
    Board() { resetBoard(); }

    // base Board
    void resetBoard();
    void setBoardTiles(
        const std::map<Position, std::deque<Tile>> &tiles); // to nie ma sensu bardzej move
    void addEmptyTilesAroundBoard();
    void moveTile(Position position, Position newPosition);
    void removeTile(Position position);
    void addTile(Position position, Tile tile);

    std::vector<std::list<Tile>> getTiles() const;
    std::set<Position> getPlayerTiles(char color) const;
    std::vector<Position> getPositions() const;

    Tile getTile(Position position) const;
    Tile getTileByNotation(const std::string &notation) const;

    int getLevel(Position position) const;
    int calculateColoredNeighbours(Position position, char color) const;
    int calculateNeighbours(const Position &position) const;

    static std::set<Position> getNeighbours(Position position);

    Position whiteQueen = invalidPosition;
    Position blackQueen = invalidPosition;
    std::map<Position, std::deque<Tile>> boardTiles;
    std::set<Position> emptyTiles;
    std::map<std::string, Position> positions;

    int getTileCount() const { return static_cast<int>(boardTiles.size()); }

    bool isEmpty(Position position) const { return boardTiles.find(position) == boardTiles.end(); }

    // movement manager
    void getAvailableMoves(char type, const Position &position, std::set<Position> &outMoves);
    void getAntMoves(const Position &position, std::set<Position> &outMoves);
    void getSpiderMoves(const Position &position, std::set<Position> &outMoves);
    void getGrasshopperMoves(const Position &position, std::set<Position> &outMoves) const;
    void getBeetleMoves(const Position &position, std::set<Position> &outMoves) const;
    void getQueenMoves(const Position &position, std::set<Position> &outMoves) const;

    // move validator
    bool isTouchingHiveAfterMove(Position position, Position newPosition) const;
    bool fredomToMove(hive::Position neighborPosition1, int level,
                      hive::Position neighborPosition2) const;
    bool constantContact(hive::Position neighborPosition1, hive::Position neighborPosition2,
                         hive::Position newPosition) const;
    bool isDirectionBlocked(Position position, Position direction, int level) const;
    bool isQueenSurrounded(char color) const;
    bool isOccupiedByOpponent(Position pos, char color) const;
    bool isMoveBlocked(Position position, Position newPosition) const;

    std::set<Position> articulationPoints;
};
} // namespace hive