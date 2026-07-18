#pragma once

#include <array>
#include <cstddef>

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
    explicit Player(char color);
    Tile takeTile(char type);

    void returnTile(char type);
    void reset();

    int getTileCount(char type) const;

    int getUnplacedPieceNumber(char type) const;

    bool queenPlaced = false;
    bool firstMove = true;
    int turnCounter = 0;

  private:
    struct PieceDefinition
    {
        char type;
        int initialCount;
    };

    static constexpr std::array<PieceDefinition, 5> pieceDefinitions = {
        PieceDefinition{'Q', 1}, PieceDefinition{'S', 2}, PieceDefinition{'B', 2},
        PieceDefinition{'G', 3}, PieceDefinition{'A', 3}};

    static std::size_t pieceIndex(char type);

    char color;
    std::array<int, pieceDefinitions.size()> pieceCounters{};
};

class Players
{
  public:
    Player &operator[](char color);
    const Player &operator[](char color) const;

  private:
    std::array<Player, 2> values{Player{'W'}, Player{'B'}};
};
} // namespace hive
