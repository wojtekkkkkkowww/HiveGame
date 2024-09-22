#pragma once

#include <memory>
#include <set>

namespace hive
{
    enum class TileType
    {
        QUEEN,
        BEETLE,
        SPIDER,
        GRASSHOPPER,
        ANT
    };

    enum class Color
    {
        WHITE,
        BLACK
    };

    inline std::pair<int, int> operator+(const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
    {
        return {lhs.first + rhs.first, lhs.second + rhs.second};
    }
    inline bool operator==(const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    class Tile
    {
    public:
        Tile() {}
        Tile(TileType type, Color color) : color(color), type(type) {}
        virtual ~Tile() = default;

        bool operator==(const Tile &other) const
        {
            return this->type == other.type && this->color == other.color && this->position == other.position;
        }
        
        void setPosition(std::pair<int, int> position) { this->position = position; }

        std::pair<int, int> position;
        Color color;
        TileType type;
    };

}
