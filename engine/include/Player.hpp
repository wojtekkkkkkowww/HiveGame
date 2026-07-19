#pragma once

#include <array>
#include <cstddef>

#include "PieceCatalog.hpp"
#include "Tile.hpp"

namespace hive
{
/*
 * @brief The Player class represents a player in the game.
 * Each player has a color and a specyfic number of tiles.
 * Take function is used to take a tile from the player's pool.
 */
class Player
{
  public:
    explicit Player(PlayerColor color);
    Tile takeTile(PieceType type);

    void returnTile(PieceType type);
    void reset();

    int getTileCount(PieceType type) const;

    int getPlacedPieceCount(PieceType type) const;

    bool queenPlaced = false;
    bool firstMove = true;
    int turnCounter = 0;

  private:
    PlayerColor color;
    std::array<int, pieceCatalog.size()> pieceCounters{};
};

class Players
{
  public:
    Player &operator[](PlayerColor color);
    const Player &operator[](PlayerColor color) const;

  private:
    std::array<Player, 2> values{Player{PlayerColor::White}, Player{PlayerColor::Black}};
};
} // namespace hive
