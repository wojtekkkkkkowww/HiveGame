#pragma once
#include <limits>
#include <array>
#include <functional>
#include <unordered_set>
#include <unordered_map>

namespace hive
{
    /*
    Data structure representing a position on the board.
    */
    struct Position
    {
        int x;
        int y;

        bool operator==(const Position &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator<(const Position &other) const
        {
            return x < other.x || (x == other.x && y < other.y);
        }

        bool operator!=(const Position &other) const
        {
            return !(*this == other);
        }

        Position operator+(const Position &other) const
        {
            return {x + other.x, y + other.y};
        }

        Position operator-(const Position &other) const
        {
            return {x - other.x, y - other.y};
        }

        Position &operator+=(const Position &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }
    };

    struct PositionHash
    {
        std::size_t operator()(const Position &pos) const
        {
            std::size_t h1 = std::hash<int>()(pos.x);
            std::size_t h2 = std::hash<int>()(pos.y);
            return h1 ^ (h2 << 1);
        }
    };

    using PositionSet = std::unordered_set<hive::Position, hive::PositionHash>;
    using PositionMap = std::unordered_map<hive::Position, int, hive::PositionHash>;

    constexpr Position invalidPosition{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    constexpr Position NW{0, -1};
    constexpr Position SE{0, 1};
    constexpr Position NE{1, -1};
    constexpr Position E{1, 0};
    constexpr Position SW{-1, 1};
    constexpr Position W{-1, 0};
    constexpr std::array<Position, 6> directions = {NW, NE, E, SE, SW, W};

}