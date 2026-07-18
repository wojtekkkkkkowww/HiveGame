#include "HexDrawable.hpp"
#include "PieceAtlas.hpp"
#include <algorithm>

HexDrawable::HexDrawable(float size) : size(size)
{
    hex.setRadius(size);
    hex.setPointCount(6);
    hex.setOrigin(size, size);
    hex.setFillColor(sf::Color::White);
    hex.setOutlineThickness(2.0f);
    hex.setOutlineColor(sf::Color(246, 184, 66));
}

void HexDrawable::setPosition(float x, float y)
{
    hex.setPosition(x, y);
    sprite.setPosition(x, y);
}

void HexDrawable::setTile(const hive::Tile &tile, const sf::Texture &atlas)
{
    if (tile.color == 'W')
    {
        hex.setFillColor(sf::Color(236, 232, 220));
    }
    else
    {
        hex.setFillColor(sf::Color(28, 27, 26));
    }

    const auto textureRect = PieceAtlas::textureRect(atlas, tile.type);
    sprite.setTexture(atlas);
    sprite.setTextureRect(textureRect);
    sprite.setOrigin(static_cast<float>(textureRect.width) / 2.0f,
                     static_cast<float>(textureRect.height) / 2.0f);
    const float targetSize = size * 1.6f;
    const float scaleX = targetSize / static_cast<float>(textureRect.width);
    const float scaleY = targetSize / static_cast<float>(textureRect.height);
    const float uniformScale = std::min(scaleX, scaleY);
    sprite.setScale(uniformScale, uniformScale);
}

void HexDrawable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(hex, states);
    target.draw(sprite, states);
}

bool HexDrawable::contains(const sf::Vector2f &point) const
{
    sf::Vector2f hexPos = hex.getPosition();
    sf::Vector2f localPoint = point - hexPos;

    float distanceSquared = localPoint.x * localPoint.x + localPoint.y * localPoint.y;
    float radiusSquared = size * size * 0.6f;
    return distanceSquared <= radiusSquared;
}
