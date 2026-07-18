#pragma once

#include "Tile.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace hive
{
struct PieceDefinition
{
    PieceType type;
    char code;
    std::uint8_t initialCount;
    std::string_view assetName;
    std::uint8_t atlasColumn;
};

inline constexpr std::array pieceCatalog = {
    PieceDefinition{PieceType::Queen, 'Q', 1, "Q.png", 0},
    PieceDefinition{PieceType::Ant, 'A', 3, "A.png", 1},
    PieceDefinition{PieceType::Spider, 'S', 2, "S.png", 2},
    PieceDefinition{PieceType::Grasshopper, 'G', 3, "G.png", 3},
    PieceDefinition{PieceType::Beetle, 'B', 2, "B.png", 4},
};

constexpr std::size_t pieceIndex(char code)
{
    for (std::size_t index = 0; index < pieceCatalog.size(); ++index)
    {
        if (pieceCatalog[index].code == code)
        {
            return index;
        }
    }
    throw std::invalid_argument("Invalid piece type");
}
} // namespace hive
