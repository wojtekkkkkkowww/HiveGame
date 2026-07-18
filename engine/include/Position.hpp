#pragma once
#include <array>
#include <limits>

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

constexpr std::array<Position, 2> neighboringDirections(Position p)
{
    if (p == NW)
        return {W, NE};
    if (p == NE)
        return {NW, E};
    if (p == E)
        return {NE, SE};
    if (p == SE)
        return {E, SW};
    if (p == SW)
        return {SE, W};
    if (p == W)
        return {SW, NW};

    return {};
}

} // namespace hive
