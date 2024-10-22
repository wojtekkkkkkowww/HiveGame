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
    constexpr Position NW{0, -1};
    constexpr Position SE{0, 1};
    constexpr Position NE{1, -1};
    constexpr Position E{1, 0};
    constexpr Position SW{-1, 1};
    constexpr Position W{-1, 0};

}