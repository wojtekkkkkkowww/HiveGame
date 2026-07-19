#pragma once

#include <cstdint>
#include <stdexcept>
#include <string_view>

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

enum class PlayerColor : std::uint8_t
{
    White,
    Black
};

enum class GameStatus : std::uint8_t
{
    Playing,
    WhiteWins,
    BlackWins,
    Draw
};

constexpr PlayerColor opposite(PlayerColor color) noexcept
{
    return color == PlayerColor::White ? PlayerColor::Black : PlayerColor::White;
}

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

constexpr char toChar(PlayerColor color) noexcept
{
    return color == PlayerColor::White ? 'W' : 'B';
}

constexpr PlayerColor playerColorFromChar(char value)
{
    switch (value)
    {
    case 'W':
        return PlayerColor::White;
    case 'B':
        return PlayerColor::Black;
    default:
        throw std::invalid_argument("Invalid player color character");
    }
}

constexpr std::string_view toString(GameStatus status) noexcept
{
    switch (status)
    {
    case GameStatus::Playing:
        return "PLAYING";
    case GameStatus::WhiteWins:
        return "WHITE_WINS";
    case GameStatus::BlackWins:
        return "BLACK_WINS";
    case GameStatus::Draw:
        return "DRAW";
    }
    return "UNKNOWN";
}
} // namespace hive
