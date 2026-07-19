#pragma once

#include "PieceCatalog.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <array>
#include <stdexcept>

class PieceAtlas
{
  public:
    static sf::IntRect textureRect(const sf::Texture &atlas, hive::PieceType pieceType)
    {
        const auto textureSize = atlas.getSize();
        if (textureSize.x < hive::pieceCatalog.size() || textureSize.y == 0)
        {
            throw std::runtime_error("Piece atlas has invalid dimensions");
        }

        const auto &definition = hive::pieceCatalog[hive::pieceIndex(pieceType)];
        const auto fullCellWidth = static_cast<int>(textureSize.x / hive::pieceCatalog.size());

        // Visible alpha bounds measured from the generated atlas. Keeping them here makes
        // centering independent of uneven transparent margins in individual cells.
        struct Bounds
        {
            int left;
            int top;
            int right;
            int bottom;
        };

        static constexpr std::array<Bounds, 5> visibleBounds = {
            Bounds{44, 177, 417, 564}, Bounds{63, 167, 397, 560}, Bounds{35, 164, 401, 560},
            Bounds{38, 164, 378, 565}, Bounds{26, 171, 366, 556}};

        constexpr int padding = 10;
        const auto bounds = visibleBounds.at(definition.atlasColumn);
        return {static_cast<int>(definition.atlasColumn) * fullCellWidth + bounds.left - padding,
                bounds.top - padding, bounds.right - bounds.left + 2 * padding,
                bounds.bottom - bounds.top + 2 * padding};
    }
};
