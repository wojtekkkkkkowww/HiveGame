#pragma once
#include <limits>


namespace hive
{
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

    };
    
    constexpr Position invalidPosition{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    constexpr Position N{0, -1};
    constexpr Position S{0, 1};
    constexpr Position NE{1, -1};
    constexpr Position SE{1, 0};
    constexpr Position SW{-1, 1};
    constexpr Position NW{-1, 0};

}