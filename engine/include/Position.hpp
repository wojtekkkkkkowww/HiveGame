#pragma once
#include <array>
#include <limits>
#include <optional>

namespace hive
{
/*
Data structure representing a position on the board.
*/
struct Position
{
    int x{};
    int y{};

    auto operator<=>(const Position &) const = default;

    constexpr Position operator+(const Position &other) const
    {
        return {.x = (x + other.x), .y = (y + other.y)};
    }

    constexpr Position operator-(const Position &other) const
    {
        return {.x = (x - other.x), .y = (y - other.y)};
    }

    constexpr Position &operator+=(const Position &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
};

constexpr Position invalidPosition{.x = std::numeric_limits<int>::max(),
                                   .y = std::numeric_limits<int>::max()};
constexpr Position NW{.x = 0, .y = -1};
constexpr Position SE{.x = 0, .y = 1};
constexpr Position NE{.x = 1, .y = -1};
constexpr Position E{.x = 1, .y = 0};
constexpr Position SW{.x = -1, .y = 1};
constexpr Position W{.x = -1, .y = 0};
constexpr std::array<Position, 6> directions = {NW, NE, E, SE, SW, W};

constexpr std::optional<std::array<Position, 2>> neighboringDirections(Position p)
{
    if (p == NW)
        return std::array<Position, 2>{W, NE};
    if (p == NE)
        return std::array<Position, 2>{NW, E};
    if (p == E)
        return std::array<Position, 2>{NE, SE};
    if (p == SE)
        return std::array<Position, 2>{E, SW};
    if (p == SW)
        return std::array<Position, 2>{SE, W};
    if (p == W)
        return std::array<Position, 2>{SW, NW};

    return std::nullopt;
}

} // namespace hive
