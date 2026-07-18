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

    struct PieceStack
    {
        [[nodiscard]] bool empty() const noexcept { return count == 0; }
        [[nodiscard]] std::size_t size() const noexcept { return count; }
        Tile &back() { return values.at(count - 1); }
        const Tile &back() const { return values.at(count - 1); }
        auto begin() noexcept { return values.begin(); }
        auto begin() const noexcept { return values.begin(); }
        auto end() noexcept { return values.begin() + static_cast<std::ptrdiff_t>(count); }
        auto end() const noexcept
        {
            return values.begin() + static_cast<std::ptrdiff_t>(count);
        }
        void push_back(Tile tile)
        {
            if (count >= values.size())
            {
                throw std::length_error("Piece stack is full");
            }
            values[count++] = std::move(tile);
        }
        void pop_back()
        {
            if (count == 0)
            {
                throw std::out_of_range("Piece stack is empty");
            }
            values[--count] = Tile{};
        }

      private:
        std::array<Tile, maxPieceCount> values{};
        std::size_t count = 0;
    };

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
    std::set<Position> emptyTiles;

    int getTileCount() const;
    std::size_t getPieceCount() const;
    bool isEmpty(Position position) const { return getLevel(position) == 0; }

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

  private:
    StaticMap<Position, PieceStack, maxPieceCount> boardTiles;
};
} // namespace hive
