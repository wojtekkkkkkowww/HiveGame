#pragma once

#include "GameTypes.hpp"
#include "Position.hpp"
#include <string>
#include <utility>

namespace hive
{

/*
 * @brief The Tile struct represents a single tile in the game.
 */
struct Tile
{
    Tile() = default;

    Tile(PieceType type, PlayerColor color) : color(color), type(type) {}

    explicit Tile(std::string notation)
        : notation(std::move(notation)), color(playerColorFromChar(this->notation.at(0))),
          type(pieceTypeFromChar(this->notation.at(1)))
    {
    }

    bool operator==(const Tile &other) const = default;

    void setPosition(Position position) { this->position = position; }

    void setNotation(std::string notation) { this->notation = std::move(notation); }

    Position position = invalidPosition;
    std::string notation;
    PlayerColor color = PlayerColor::White;
    PieceType type = PieceType::Queen;
    bool placed = false;
};

} // namespace hive
