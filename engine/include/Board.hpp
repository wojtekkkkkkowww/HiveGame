#pragma once

#include "Position.hpp"
#include "StaticMap.hpp"
#include "Tile.hpp"
#include <array>
#include <cstdint>
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
    static constexpr std::size_t maxPieceCount = 22;

    using PieceStack = std::deque<Tile>;

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
    std::set<Position> getPlayerTiles(PlayerColor color) const;
    std::vector<Position> getPositions() const;

    Tile getTile(Position position) const;
    Tile getTileByNotation(const std::string &notation) const;

    int getLevel(Position position) const;
    int calculateColoredNeighbours(Position position, PlayerColor color) const;
    int calculateNeighbours(const Position &position) const;

    static constexpr std::array<Position, 6> getNeighbours(Position position)
    {
        // Keep the former std::set lexicographic order so serialized replays stay stable.
        return {position + W,  position + SW, position + NW,
                position + SE, position + NE, position + E};
    }

    Position whiteQueen = invalidPosition;
    Position blackQueen = invalidPosition;
    std::set<Position> emptyTiles;

    int getTileCount() const;
    std::size_t getPieceCount() const;

    bool isEmpty(Position position) const { return getLevel(position) == 0; }

    // movement manager
    void getAvailableMoves(PieceType type, const Position &position, std::set<Position> &outMoves);
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
    bool isQueenSurrounded(PlayerColor color) const;
    bool isOccupiedByOpponent(Position pos, PlayerColor color) const;
    bool isMoveBlocked(Position position, Position newPosition) const;

    std::set<Position> articulationPoints;

  private:
#if HIVE_USE_STATIC_MAP
    using BoardStorage = StaticMap<Position, PieceStack, maxPieceCount>;
#else
    using BoardStorage = std::map<Position, PieceStack>;
#endif

    BoardStorage boardTiles;
};
} // namespace hive
