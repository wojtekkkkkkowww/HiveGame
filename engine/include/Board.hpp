#pragma once

#include "PieceCatalog.hpp"
#include "Position.hpp"
#include "StaticContainers.hpp"
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
    static constexpr std::size_t beetlesPerPlayer = []() constexpr {
        for (const auto &definition : pieceCatalog)
        {
            if (definition.type == PieceType::Beetle)
            {
                return static_cast<std::size_t>(definition.initialCount);
            }
        }
        return std::size_t{0};
    }();
    static_assert(beetlesPerPlayer > 0, "Piece catalog must define beetles");
    static constexpr std::size_t maxStackHeight = (2 * beetlesPerPlayer) + 1;
    static constexpr std::size_t neighborDirections = directions.size();
    // For a connected hive on hex grid, the number of distinct empty neighbor cells
    // around occupied positions is bounded by 2N + 4, where N is occupied positions.
    static constexpr std::size_t maxConnectedEmptyNeighbors = (2 * maxPieceCount) + 4;
    static constexpr std::size_t maxTraversalPositions = maxConnectedEmptyNeighbors + 1;

    using PieceStack = StaticVector<Tile, maxStackHeight>;
    using PositionSet = StaticSet<Position, maxConnectedEmptyNeighbors>;
    using PlayerTileSet = StaticSet<Position, maxPieceCount>;

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
    PlayerTileSet getPlayerTiles(PlayerColor color) const;
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
    PositionSet emptyTiles;

    int getTileCount() const;
    std::size_t getPieceCount() const;

    bool isEmpty(Position position) const { return getLevel(position) == 0; }

    // movement manager
    void getAvailableMoves(PieceType type, const Position &position, PositionSet &outMoves);
    void getAvailableMoves(PieceType type, const Position &position, std::set<Position> &outMoves);
    void getAntMoves(const Position &position, PositionSet &outMoves);
    void getSpiderMoves(const Position &position, PositionSet &outMoves);
    void getGrasshopperMoves(const Position &position, PositionSet &outMoves) const;
    void getBeetleMoves(const Position &position, PositionSet &outMoves) const;
    void getQueenMoves(const Position &position, PositionSet &outMoves) const;

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

    PlayerTileSet articulationPoints;

  private:
    using BoardStorage = StaticMap<Position, PieceStack, maxPieceCount>;

    BoardStorage boardTiles;
};
} // namespace hive
