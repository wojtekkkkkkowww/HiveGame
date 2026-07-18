#pragma once

#include "Position.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

namespace hive
{

enum class PieceType : std::uint8_t
{
    Queen,
    Ant,
    Spider,
    Grasshopper,
    Beetle
};

enum class PieceColor : std::uint8_t
{
    White,
    Black
};

constexpr char toChar(PieceType type)
{
    switch (type)
    {
    case PieceType::Queen:
        return 'Q';
    case PieceType::Ant:
        return 'A';
    case PieceType::Spider:
        return 'S';
    case PieceType::Grasshopper:
        return 'G';
    case PieceType::Beetle:
        return 'B';
    }
    throw std::invalid_argument("Invalid piece type");
}

constexpr PieceType pieceTypeFromChar(char value)
{
    switch (value)
    {
    case 'Q':
        return PieceType::Queen;
    case 'A':
        return PieceType::Ant;
    case 'S':
        return PieceType::Spider;
    case 'G':
        return PieceType::Grasshopper;
    case 'B':
        return PieceType::Beetle;
    default:
        throw std::invalid_argument("Invalid piece type character");
    }
}

constexpr char toChar(PieceColor color)
{
    switch (color)
    {
    case PieceColor::White:
        return 'W';
    case PieceColor::Black:
        return 'B';
    }
    throw std::invalid_argument("Invalid piece color");
}

constexpr PieceColor pieceColorFromChar(char value)
{
    switch (value)
    {
    case 'W':
        return PieceColor::White;
    case 'B':
        return PieceColor::Black;
    default:
        throw std::invalid_argument("Invalid piece color character");
    }
}

/*
 * @brief The Tile struct represents a single tile in the game.
 */
struct Tile
{
    Tile() = default;
    Tile(char type, char color) : color(color), type(type) {}
    explicit Tile(std::string notation)
        : notation(std::move(notation)),
          color(this->notation.size() >= 1 ? this->notation[0] : '\0'),
          type(this->notation.size() >= 2 ? this->notation[1] : '\0')
    {
    }

    bool operator==(const Tile &other) const = default;

    void setPosition(Position position) { this->position = position; }

    void setNotation(std::string notation) { this->notation = std::move(notation); }

    Position position = invalidPosition;
    std::string notation;
    char color = '\0';
    char type = '\0';
    bool placed = false;
};

} // namespace hive
